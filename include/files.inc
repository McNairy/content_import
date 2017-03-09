<?php

function attach_file($path, $node, $file, $mime_list)
{
  //TODO: only process files of specific mime-types--switch?
  switch($file)
  {
    case "all":
      $attachment = file_get_contents($path);
      $file = file_save_data($attachment, 'public://' . drupal_basename($path),FILE_EXISTS_REPLACE);
      $file->display = 1;
      $file->description = "";
      $node->field_attachment = array(
        LANGUAGE_NONE => array(
          '0' => (array)$file
        )
      );
    break;
    case "some":
      //$mime_list = array('txt', 'rtf', 'jpeg', 'jpg', 'png', 'pdf', 'doc', 'docx', 'xls', 'xlsx');
      foreach ($mime_list as $mime) 
      {
        if(check_mime_type($mime, $mime_list))
        {
          $attachment             = file_get_contents($path);
          $file                   = file_save_data($attachment, 'public://' . drupal_basename($path),FILE_EXISTS_REPLACE);
          $file->display          = 1;
          $file->description      = "";
          $node->field_attachment = array(
            LANGUAGE_NONE => array(
              '0' => (array)$file
            )
          );
        }
      }
    break;
    default:
  }
}

function get_mime_type($url) {
    $buffer = file_get_contents($url);
    $finfo = new finfo(FILEINFO_MIME_TYPE);
    $type = preg_replace('%^[a-z]+\/%', '', $finfo->buffer($buffer));
    return $type;
}

/**
 * Check mime type of attachments
 * @param  [type] $urls  [description]
 * @param  [type] $types [description]
 * @return [type]        [description]
 */
function check_mime_type($urls, $types)
{
  //$types = array('pdf', 'doc', 'docx', 'png', 'jpg');
  foreach($types as $t)
  {
    if(get_mime_type($urls) == $t)
    {
      return true;
    } else {
      return false;
    }
  }
}
