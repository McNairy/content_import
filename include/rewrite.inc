<?php

function get_rewrites($rid = null) {
  if($rid == null) {
    $keys       = db_query("SELECT rid FROM content_import_rewrite")->fetchCol();
    $values     = db_query("SELECT rewrite_name FROM content_import_rewrite")->fetchCol();
    $results    = array_combine($keys, $values);
    $results[0] = "Select"; //safe to add index of zero, the database results will never have an index of zero
    ksort($results);

    return $results;
  }

  $result = db_select('content_import_rewrite', 'c')
    ->fields('c')
    ->condition('rid', $rid, '=')
    ->execute()
    ->fetchAssoc();

  return $result;
}

/**
 * Get rewrite bodies for a content import batch
 * @param  string $batch_name 
 * @return string Contactenated list of rewrites
 */
function get_rewrite_bodies($batch_name) {
  $query = db_select('content_import_rewrite', 'rewrite');
  $query->join('content_import_log', 'log', 'rewrite.nid = log.nid');
  $query
    ->condition('log.batch_name', $batch_name, '=')
    ->fields('rewrite', array('rid', 'rewrite_body'));

  $rewrites = $query->execute()->fetchAll();
  $bodies   = "";

  foreach ($rewrites as $rewrite) {
    $bodies .= $rewrite->rewrite_body . PHP_EOL;
  }

  return $bodies;
}

/**
 * Create an Internet Information Server rewrite rule
 * @param  string $title  Rewrite name
 * @param  string $oldUrl Old URL address
 * @param  string $newUrl New URL address
 * @return string Rewriterule
 */
function create_iis_rewrite($title, $oldUrl, $newUrl) {
    $old         = parse_url($oldUrl);
    $old['path'] = ltrim($old['path'], '/');
    $rule        = <<< RULE
<rule name="{$title}" patternSyntax="ExactMatch" stopProcessing="true">
    <match url="{$old['path']}" />
    <action type="Redirect" url="{$newUrl}" />
</rule>
RULE;

    return $rule;
}

/**
 * Create an Apache HTTPD redirect
 * @param  string $title  Redirect description
 * @param  string $oldUrl Old URL address
 * @param  string $newUrl New URL address
 * @return string Redirect rule
 */
function create_apache_redirect($title, $oldUrl, $newUrl) {
    $old  = parse_url($oldUrl);
    $rule = <<< RULE
        #{$title}
        Redirect {$oldUrl} {$newUrl}
RULE;

    return $rule;
}

/**
 * Download redirects to add to origin web server
 * @param  [type] $rule [description]
 * @return [type]       [description]
 */
function download_rewrites($filename, $data) {
  header('Content-Description: File Transfer');
  header("Content-type: text/plain");
  header("Content-Disposition: attachment; filename={$filename}");
  header('Expires: 0');
  header('Cache-Control: must-revalidate');
  header('Pragma: public');
  flush();
  echo $data;
}

/**
 * Create redirect for original web server
 * @param  [type] $type [description]
 * @return [type]       [description]
 */
function create_redirect($nid, $type, $title, $url, $alias) {
  switch ($type) {
    case 'Apache':
      $rule = create_apache_redirect($title, $url, $alias);
      break;
    case 'IIS':
      $rule = create_iis_rewrite($title, $url, $alias);
      break;
    case 'None':
      return;
  }
    return add_rewrite($nid, $title, $rule);
}

function add_rewrite($nid, $title, $rule) {
  $table  = "content_import_rewrite";
  $record = array(
    'nid'          => $nid,
    'rewrite_name' => $title,
    'rewrite_body' => $rule,
  );

  return drupal_write_record($table, $record);
}

function add_url($nid, $url) {
  $table  = "content_import_tbd";
  $record = array(
    'nid' => $nid,
    'url' => $url,
  );

  drupal_write_record($table, $record);
}

function download_rewrites_submit($form, &$form_state) {
  $batch_name = $form_state['values']['rewrite_name'];
  if($batch_name == "Select") {
    drupal_set_message('Selected rewrite is not valid.', 'error');
    return;
  }
  $batch_name = $form_state['values']['rewrite_name'];
  $content    = get_rewrite_bodies($batch_name);
  $batch_name = str_replace(' ', '-', $batch_name);
  $filename   = "rewrite-rules-" . $batch_name . ".txt";

  download_rewrites($filename, $content);
}
