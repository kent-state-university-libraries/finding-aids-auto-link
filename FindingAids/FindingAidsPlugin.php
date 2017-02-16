<?php

/**
 * Finding Aids Plugin
 * Sends item information to another system that stores finding aid, and links the finding aid to the respective item record.
 *
 * @author Joe Corall <jcorall@kent.edu>
 */

class FindingAidsPlugin extends Omeka_Plugin_AbstractPlugin
{
    protected $_hooks = array(
        'after_save_item',
        'config_form',
        'config',
    );

    public function hookAfterSaveItem($args)
    {
        $item = $args['record'];
        if ($item->public == 1) {
            // find what element stores the finding aid URL (set in the plugin config)
            $element_id = get_option('finding_aids_element');
            // if it's not set, abort
            if (is_null($element_id)) {
                return;
            }

            // get the element name and element set name so we can call metadata()
            $db = get_db();
            $element = $db->query("SELECT e.name, s.name AS element_set
              FROM {$db->Element} e
              INNER JOIN {$db->ElementSet} s ON s.id = e.element_set_id
              WHERE e.id = ?", $element_id)->fetchObject();
            $finding_aid_url = metadata($item, array($element->element_set, $element->name));
            // if no value set for this item record for the respective element, abort
            if (empty($finding_aid_url)) {
                return;
            }

            // get some more metadata for this item to pass along with the request
            // this metadata will be used by the endpoint to find what text to link
            $title = metadata($item, array('Dublin Core', 'Title'));
            $date = metadata($item, array('Dublin Core', 'Date'));
            if (empty($date)) {
                $date = 'Undated';
            }

            // get the proper URL whether on prod or test
            $option = APPLICATION_ENV === 'production' ? 'finding_aids_production_endpoint' : 'finding_aids_test_endpoint';
            $endpoint_url = get_option($option);
            if (is_null($endpoint_url)) {
                return;
            }

            // send the information to the endpoint
            $client = new Zend_Http_Client($endpoint_url);
            $client->setHeaders('finding-aid-secret-key', get_option('finding_aids_secret_key'));
            $client->setParameterGet('i', $item->id);
            $client->setParameterGet('t', $title);
            $client->setParameterGet('d', $date);
            $client->setParameterGet('l', $finding_aid_url);
            $response = $client->request('GET');

            $error = $response->getBody();
            if ($error) {
                // @todo how to handle?
            }
        }
    }

    /**
     * Config Form for this plugin
     *
     * The logic that populates $options is taken from the BulkMetadataEditor Plugin
     * @see BulkMetadataEditor_IndexController::_getFormElementOptions()
     */
    public function hookConfigForm()
    {
        // get all elements in the system grouped by element set
        $db = get_db();
        $sql = "SELECT es.name AS element_set_name, e.id AS element_id,
            e.name AS element_name, it.name AS item_type_name
            FROM {$db->ElementSet} es
            INNER JOIN {$db->Element} e ON es.id = e.element_set_id
            LEFT JOIN {$db->ItemTypesElements} ite ON e.id = ite.element_id
            LEFT JOIN {$db->ItemType} it ON ite.item_type_id = it.id
            WHERE es.record_type IS NULL
                OR es.record_type = 'Item'
            ORDER BY es.name, it.name, e.name";
        $elements = $db->fetchAll($sql);
        $options = array('- Select -');
        foreach ($elements as $element) {
            $optGroup = $element['item_type_name']
                      ? __('Item Type') . ': ' . __($element['item_type_name'])
                      : __($element['element_set_name']);
            $value = __($element['element_name']);

            $options[$optGroup][$element['element_id']] = $value;
        }

        require_once 'config-form.php';
    }

    /**
     * Save the config form results
     */
    public function hookConfig()
    {
        $options = array(
            'finding_aids_production_endpoint',
            'finding_aids_test_endpoint',
            'finding_aids_element',
            'finding_aids_secret_key',
        );
        foreach ($options as $option) {
            if (empty($_POST[$option])) {
                delete_option($option);
            }
            else {
                set_option($option, $_POST[$option]);
            }
        }
    }
}
