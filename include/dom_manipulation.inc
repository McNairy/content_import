<?php

/**
 * Remove attributes from HTML elements
 * @param  string $html
 * @param  array $notAllowedAttributes Array of HTML attributes to strip. Defaults to all.
 * @param  array $allowedAttributes Array of HTML attributes not to strip.
 * @return string Returns HTML without the stripped HTML attribues
 */
function strip_attributes($html, $notAllowedAttributes, $allowedAttributes) {
  $sa = new StripAttributes();

  foreach($notAllowedAttributes as $na) {
      if(in_array($na, $allowedAttributes))
      {
          for($i = 0; $i < count($allowedAttributes); $i++)
          {
              if($allowedAttributes[$i] == $na)
              {
                  unset($allowedAttributes[$i]);
              } 
          }
      }
  }

  $sa->allow = $allowedAttributes;
  $output = $sa->strip($html);

  return $output;
}

/**
 * Parses form datat and returns a string to be used for the node's title.
 * @param  string $file   A URL.
 * @param  array $values An array of submitted form values.
 * @return string  String to be used for node title.
 */
function get_title($file, $values) {
  $html = new simple_html_dom();
  $html->load_file($file);
  switch ($values['title-options']) {
    case 'default':
        $output = $html->find('title', 0);
        break;
    case 'alt-class':
        $output = $html->find("." . $values['alt-class'], 0);
        break;
    case 'alt-div':
        $output = $html->find("#" . $values['alt-div'], 0);
        break;
  }

  return $output->plaintext;
}

/**
 * Strip element from content source
 * @param  string  $file    URL address
 * @param  string  $element HTML element name
 * @param  integer $depth   How far to descend into DOM 
 * @return string Stripped HTML
 */
function get_element($file, $element, $depth = 0) {
  $element = explode(' ', $element);
  $output  = '';
  $html    = new simple_html_dom();
  $html->load_file($file);

  foreach($element as $div)
  {
      $output .= $html->getElementById("{$div}");
  }

  return $output;
}

/**
 * Get list of HTML tags
 * @return array Array of HTML tags
 */
function get_html_tags()
{
  $list = array();
  $vocabs = taxonomy_get_vocabularies();
  foreach($vocabs as $v)
  {
    if($v->machine_name == "html_tags")
    {
      $tree = taxonomy_get_tree($v->vid);
      foreach($tree as $t)
      {
        $list[] = $t->name;
      }
    }
  }

  return $list;
}
