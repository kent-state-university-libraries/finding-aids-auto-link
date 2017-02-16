<?php

function finding_aid_admin_form($form, &$form_state) {
  $form['finding_aid_allowed_servers'] = array(
    '#type' => 'textarea',
    '#title' => t('IP Addresses with link updating access'),
    '#description' => t('This should only be the IP address of the servers that need to update finding aids. Adding an IP address here essentially gives this IP address (very limited) write access to all of your finding aids. One IP address per line.'),
    '#attributes' => array('placeholder' => "10.0.0.1\n192.168.1.1\n192.168.100.1"),
    '#required' => TRUE,
    '#default_value' => implode("\n", variable_get('finding_aid_allowed_servers', array()))
  );

  $sql = 'SELECT DISTINCT(n.type), t.name FROM {node} n
    INNER JOIN {node_type} t ON t.type = n.type
    WHERE n.status = 1
    ORDER BY n.type';
  $form['finding_aid_content_type'] = array(
    '#type' => 'select',
    '#title' => t('Content Type'),
    '#description' => t('The content type that your finding aids are added as'),
    '#options' => db_query($sql)->fetchAllKeyed(),
    '#default_value' => variable_get('finding_aid_content_type'),
    '#required' => TRUE,
  );
  $form['finding_aid_secret_key'] = array(
    '#type' => 'key_select',
    '#title' => t('Secret key'),
    '#description' => t('The key value found in the selected key will be required when sending requests to update a finding aid. If the desired key is not listed, <a href="/admin/config/system/keys/add">create a new key</a>.'),
    '#key_description' => FALSE,
    '#required' => TRUE,
    '#default_value' => variable_get('finding_aid_secret_key', FALSE),
  );
  $form['finding_aid_omeka_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Omeka URL'),
    '#description' => t('The base URL for your Omeka instance'),
    '#default_value' => variable_get('finding_aid_omeka_url', ''),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

function finding_aid_admin_form_validate($form, &$form_state) {
  $ips = explode("\n", $form_state['values']['finding_aid_allowed_servers']);
  array_walk($ips, function (&$value) { $value = trim($value); });
  $pass = TRUE;
  foreach ($ips as $key => $ip) {
    if (empty($ip)) {
      unset($ips[$key]);
    }
    elseif (!filter_var($ip, FILTER_VALIDATE_IP)) {
      form_set_error('finding_aid_allowed_servers', t('@ip is not a valid IP address.', array('@ip' => $ip)));
      $pass = FALSE;
    }
  }

  if ($pass) {
    $form_state['values']['finding_aid_allowed_servers'] = $ips;
  }
}

function finding_aid_admin_form_submit($form, &$form_state) {
  $variables = array(
    'finding_aid_allowed_servers',
    'finding_aid_content_type',
    'finding_aid_secret_key',
    'finding_aid_omeka_url',
  );
  foreach ($variables as $variable) {
    variable_set($variable, $form_state['values'][$variable]);
  }
}
