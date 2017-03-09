<?php

function add_batch_log($nid, $batch_name) 
{
  $table = "content_import_log";
  $record = array(
    'nid'        => $nid,
    'batch_name' => $batch_name
  );

  drupal_write_record($table, $record);

  return;
}

function get_batch_names()
{
  $result = db_query('SELECT batch_name FROM {content_import_log}', array());

  $names = $result->fetchAllAssoc('batch_name');

  $batch = array();

  foreach ($names as $name)
  {
    $batch[] =  $name->batch_name;
  }

  array_unshift($batch, 'Select');

  return $batch;
}

function get_batch_nids($batch_name)
{
  $result = db_query('SELECT nid FROM {content_import_log}', array(':batch_name' => $batch_name));

  $nids = $result->fetchAllAssoc('nid');

  $batch = array();

  foreach ($nids as $nid)
  {
    $batch[] =  $nid->nid;
  }

  return $batch;
}

function get_batch_lids($batch_name)
{
  $result = db_query('SELECT lid FROM {content_import_log}', array(':batch_name' => $batch_name));

  $lids = $result->fetchAllAssoc('lid');

  $batch = array();

  foreach ($lids as $lid)
  {
    $batch[] =  $lid->lid;
  }

  return $batch;
}

function delete_batch_nodes($form, &$form_state)
{
  $batch_name = $form_state['values']['batch_name'];
  $batch      = get_batch_nids($batch_name);

  foreach($batch as $b)
  {
    node_delete($b);
  }

  //clear batch from log
  try {
    delete_batch($batch_name);
    drupal_set_message('Batch deleted successfully', 'status');
  } catch (Exception $e) {
    drupal_set_message($e->getmessage(), 'error');
  }
}

function delete_batch($batch_name)
{
  $batch = get_batch_lids($batch_name);

  foreach($batch as $b) {
    $num_deleted = db_delete('content_import_log')
      ->condition('batch_name', $batch_name)
      ->execute();
  }
}

function build_keys($arr) {

}
