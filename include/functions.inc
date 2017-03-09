<?php

/**
 * Parse a list of URLs submitted via a textarea and put them into an array
 */
function get_urls($url_list)
{
  //put list of URLS in textarea into an array
  //TODO: check for empty array and return message
  $urls = preg_split("/[\r\n,]+/", $url_list);

  return $urls;
}

/**
 * Build menu list
 * @return object Menu
 */
function get_menu_list() {
  $menu = new BuildMenu();
  return $menu->get_menu_list();
}

/**
 * Extracts anchor tags from html, does not include mailto links
 * @param  string $content HTML code
 * @param  string $prepend Prepend to beginning of link, usually a domain name
 * @return array  Array of links
 */
function extract_urls($content, $prepend = null) {
    $html = new simple_html_dom();

    //array to store links
    $links = array();

    //load html from a string
    $html->load($content);

    //iterate over each anchor
    foreach($html->find('a') as $element) {
        $link = $element->href;

        //we don't care about bookmarks or email links, if found
        //skip the rest of the current iteration
        if(is_email_link($link) || is_bookmark_link($link)) {
            continue;
        }

        if(is_absolute_link($link)) {
            //add href to an array of links
            //skip the rest of the iteration
            array_push($links, $link);
            continue;
        } 

        //relative links will start with slash
        //remove starting slash then add back to standardize relative links
        $link = preg_replace('%^\/%', '', $link);
        $link = "/" . $link;

        //prepend relative link with domain name
        if($prepend != null) {
            $link = $prepend . $link;
        }

        //add href to an array of links
        array_push($links, $link);
    }

    return $links;
}

function is_email_link($link) {
        if(substr($link, 0, 6) == "mailto") {
            return true;
        }

        return false;
}

function is_bookmark_link($link) {
        if(substr($link, 0, 1) == '#') {
            return true;
        }

        return false;
}

function is_relative_link($link) {
    if(preg_match('%^\/%', $link)) {
        return true;
    }

    return false;
}

function is_absolute_link($link) {
    if(substr($link, 0, 4) == "http") {
        return true;
    }

    return false;
}

/**
 * "Drupalize" those links as if already brought in by content_import
 * Check if that path exists
 * @param  [type] $url [description]
 * @return [type]      [description]
 */
function check_url_exists($url) {
   if (!drupal_valid_path($path)) {
      // Not a system URL.
      if (!drupal_lookup_path('source', $path))  {
        /*
        --if path doesn't exist
          ---add original link to "to be imported" list available in admin section
          ---rewrite link, we are going to assume it will be imported later
        */
      }
    }
}

function content_import_node_delete($node) {
  $records = db_select('content_import_rewrite', 'c')
    ->fields('c')
    ->condition('nid', $node->nid, '=')
    ->execute()
    ->fetchAssoc();

  if(!empty($records)) {
    foreach($records as $record) {
      db_delete('content_import_rewrite')
        ->condition('rid', $record['rid'], '=')
        ->execute();

        drupal_set_message('Deleted rewrite record, rid:  ' . $record['rid']);
    }
  }
}

/**
 * TODO
Check if links already imported
-"Drupalize" those links as if already brought in by content_import
-Check if that path exists
--if path doesn't exist
---add link to "to be imported" list available in admin section
---rewrite link, we are going to assume it will be imported later
 */
