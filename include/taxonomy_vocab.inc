<?php

function get_taxonomy_vocabularies () {
  $taxonomies = taxonomy_get_vocabularies();
  $vocabularies = array();
  foreach($taxonomies as $t)
  {
    // array('return_value1' => t('Display Value 1')
    //$vocabularies['name'] = $t->name;
    // $vocabularies[] = $t->vid;
    $vocabularies[$t->vid] = $t->name;
  }

  return $vocabularies;
}

function get_vocabulary_list ($vid)
{
  //get all of the taxonomy vocabularies
  $vocabularies = taxonomy_get_vocabularies();

  //get a  vocabulary specified by vocabulary id argument
  $v = $vocabularies[$vid]->vid;

  //get the  taxonomy tree of the specified vocabulary
  $trees = taxonomy_get_tree($v);

  /*
  //store list of vocabulary items
  $key = array();
  $value = array();
  

  //iterate through taxonomy tree and add item name to list
  foreach ($trees as $tree)
  {
    $key[]  = "$tree->vid";
    $value[] = "$tree->name";
  }

  //return array_combine($key, $value);
  return array('foo', 'bar');
  */
}
