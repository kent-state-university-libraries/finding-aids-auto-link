<?php

/**
 * Update the text in a finding aid to link to another third-party system.
 *
 * printing "0" means the text was successfully linked, any other number is a failure of some sort
 *
 * @todo document return codes
 */
function finding_aid_link($second_try = FALSE) {
  drupal_add_http_header('Content-type', 'text/plain');

  // make sure all the required metadata is provided
  if (empty($_GET['i'])) {
    echo 1;
  }
  elseif(empty($_GET['t'])) {
    echo 2;
  }
  elseif(empty($_GET['l'])) {
    echo 3;
  }
  else {
    $omeka_item_id = $_GET['i'];
    $omeka_title = trim($_GET['t']);
    $date_components = explode('-', trim($_GET['d']));
    if (!$second_try) {
      if (count($date_components) == 3) {
        $time = strtotime($date_components[1] . '/' . $date_components[2] . '/' . $date_components[0]);
        $omeka_title .= ': '. date('F j, Y', $time);
      }
      elseif (count($date_components) == 2) {
        $time = mktime(0, 0, 0, $date_components[1], 1, $date_components[0]);
        $omeka_title .= ': '. date('F Y', $time);
      }
      elseif ($date_components[0] == 'Undated') {
        $omeka_title .= ': Undated';
      }
    }

    $link = $_GET['l'];
    // if this is a full URL (which it more than likely is)
    // get the path
    if (strpos($link, 'http') !== FALSE) {
      $link = parse_url($link);
      $alias = $link['path'];
    }
    $alias = trim($alias, '/');
    if (strpos($alias, 'node/') !== FALSE) {
      $nid = substr($alias, 5);
    }
    else {
      $nid = db_query('SELECT RIGHT(source, 5) FROM {url_alias} WHERE alias = :alias', array(':alias' => $alias))->fetchField();
      if (!$nid) {
        $nid = db_query('SELECT RIGHT(redirect, 5) FROM {redirect} WHERE source = :alias', array(':alias' => $alias))->fetchField();
      }
    }

    if (empty($nid)) {
      echo 4;
    }
    else {
      $node = node_load($nid);
      $content_type = variable_get('finding_aid_content_type', FALSE);
      if (empty($node)) {
        echo 5;
      }
      elseif (empty($node->status)) {
        echo 6;
      }
      elseif ($node->type !== $content_type) {
        echo 7;
      }
      else {
        $omeka_title = trim($omeka_title);
        $node->body[LANGUAGE_NONE][0]['value'] = str_replace('&nbsp;', ' ', $node->body[LANGUAGE_NONE][0]['value']);
        $count = substr_count($node->body[LANGUAGE_NONE][0]['value'], $omeka_title);
        if ($count === 0) {
          if ($second_try) {
            echo 8;
          }
          else {
            ul_finding_aid_link(TRUE);
          }
        }
        elseif ($count > 1) {
          // after two attempts, we still couldn't find a unique piece of text in this finding aid
          // based on the title/date. Give up.
          if ($second_try) {
            echo 9;
          }
          // we couldn't find a unique piece of text on the first attempt, try this a second time
          else {
            ul_finding_aid_link(TRUE);
          }
        }
        // GREAT SUCCESS
        else {
          $omeka_url = variable_get('finding_aid_omeka_url', FALSE);
          // no URL was provided in the UI.
          if (!$omeka_url) {
            echo -1;
          }
          else {
            $anchor_tag = '<a href="'
              . $omeka_url . '/'
              . $omeka_item_id
              . '" target="_blank">'
              . $omeka_title
              . '</a>';

            if (substr_count($node->body[LANGUAGE_NONE][0]['value'], $anchor_tag) === 0) {
              $spaceless_body = str_replace(array('&nbsp;', ' ', "\n", "\r"), array('', '', '', ''), $node->body[LANGUAGE_NONE][0]['value']);
              $spaceless_title = str_replace(array('&nbsp;', ' '), array('', ''), $omeka_title);
              if (substr_count($spaceless_body, $spaceless_title . '</a>') === 1) {
                echo 11;
              }
              else {
                $node->body[LANGUAGE_NONE][0]['value'] = str_replace($omeka_title, $anchor_tag, $node->body[LANGUAGE_NONE][0]['value']);

                $node->revision = 1;
                $node->revision_uid = 1330;
                $node->log = 'Linking ' . $omeka_title . ' to Omeka item ID #'.$omeka_item_id.'.';
                node_save($node);
                echo 0;
              }
            }
            // already processed
            else {
              echo 0;
            }
          }
        }
      }
    }
  }

  drupal_exit();
}
