<?php

/**
 * Implements hook_help.
 *
 * Displays help and module information.
 *
 * @param path 
 *   Which path of the site we're using to display help
 * @param arg 
 *   Array that holds the current path as returned from arg() function
 */
function content_import_help($path, $arg) {
  switch ($path) {
    case "admin/help#content_import":
      return '<p>' . t("Imports links and creates nodes form them.") . '</p>';
      break;
  }
} 

/**
 * Implements hook_block_info().
 */

function content_import_block_info() {
  $blocks['content_import'] = array(
    'info' => t('Content Import'), //The name that will appear in the block list.
    'cache' => DRUPAL_CACHE_PER_ROLE, //Default
  );
  return $blocks;
}

/**
 * Implements hook_menu().
 */
function content_import_menu() {
  $items = array();

  $items['admin/config/content/content_import'] = array(
    'title'            => 'Content Import',
    'description'      => 'Configuration for Content Import module',
    'page callback'    => 'drupal_get_form',
    'access callback'  => TRUE,
    'page arguments'   => array('content_import_config_form'),
    'access arguments' => array('access administration pages'),
    'type'             => MENU_NORMAL_ITEM,
  );

  $items['admin/config/content/content_import/rules'] = array(
    'title'            => 'Content Import Rewrite Rules',
    'description'      => 'Rewrite rules created by content_import module',
    'page callback'    => 'drupal_get_form',
    'access callback'  => TRUE,
    'page arguments'   => array('content_import_rules_form'),
    'access arguments' => array('access administration pages'),
    'type'             => MENU_NORMAL_ITEM,
  );

  $items['content_import'] = array(
    'title'              => 'Content Import',
    'description'        => 'Set Parameters for Content Importation',
    'page callback'      => 'drupal_get_form',
    'access callback'    => TRUE,
    'page arguments'     => array('content_import_form'),
    //'access arguments' => array('access administration pages'),
    'type'               => MENU_NORMAL_ITEM,
  );



  return $items;
}

/**
 * Page callback: Content Import settings
 *
 * @see content_import_menu()
 */

function content_import_form($form, &$form_state) {
  $batch_name    = time();
  $content_types = node_type_get_names();

  $form['urls'] = array(
    "#type"        => 'fieldset',
    "#title"       => t('URLs'),
    '#weight'      => 4, 
    '#collapsible' => TRUE, 
    '#collapsed'   => FALSE,
  );

  $form['urls']['batch_name'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Enter batch name'),
    '#description'   => t('Human readable name for batch job. Default is seconds since the Unix Epoch.'),
    '#default_value' => t($batch_name)
  );

  $form['urls']['url_list'] = array(
    '#type'          => 'textarea', 
    '#title'         => t('URL(s) to page crawl for links'), 
    '#description'   => t('Enter one URL per line must include protocol moniker (ex.: http://, ftp://, smb://, etc.)'),
    '#required'      => TRUE,
    '#default_value' => t("http://localhost/index.html")
  );


  $form['urls']['content_type'] = array(
     '#type'          => 'select',
     '#title'         => t('Select Content Type'),
     '#options'       => $content_types,
     '#default_value' => "page"
  );

  $form['urls']['menu_list'] = array(
    '#title'       => 'Select Menu',
    '#description' => 'General only used to assign a node of Basic content type to a menu',
    '#type'        => 'select',
    '#prefix'      => '<div id="menu-list-div">',
    '#suffix'      => '</div>',
    '#options'     => get_menu_list()
  );

  $form['urls']['enable_menu'] = array(
    '#type'          => 'checkbox',
    '#title'         => 'Enable',
    '#description'   => t('Enable menu when node is created.'),
    '#default_value' => 0
  );

  // $form['attachments'] = array(
  //   "#type" => 'fieldset',
  //   "#title" => t('Attachments'),
  //   '#weight' => 5, 
  //   '#collapsible' => TRUE, 
  //   '#collapsed' => FALSE,
  // );

  // $form['attachments']['get_file'] = array(
  //   '#type' => 'radios',
  //   '#title' => t('Get Attachments'),
  //   '#description' => t('Download images, documents, audio files, etcetera.'),
  //   '#options' => drupal_map_assoc(
  //     array(
  //       t('none'),
  //       t('all'),
  //       t('some (buggy)')
  //     )
  //   ),
  //   '#default_value' => t('none')
  // );

  // $form['attachments']['custom_files'] = array(
  //   '#type' => 'textfield',
  //   '#title' => t('Enter file extension types to be downloaded separated by commas'),
  //   '#description' => t('example: jpg, pdf, tiff'),
  //   '#states' => array(
  //   'invisible' => array(
  //    ':input[name="get_file"]' => array('value' => 'none', 'value' => 'all'),
  //   ),
  //   'visible' => array(
  //   ':input[name="get_file"]' => array('value' => 'some')
  //   ),
  // ),
  // );

$form['title'] = array(
  '#type'        => 'fieldset',
  '#title'       => 'Title',
  '#collapsible' => TRUE,
  '#collapsed'   => FALSE
);

$form['title']['title-options'] = array(
    '#type' => 'radios',
    '#title' => 'Select source to be used as title for new node',
    '#options' => [
    'default'   => 'Default', 
    'alt-class' => 'Class', 
    'alt-div'   => 'Div'
  ],
  '#default_value' => 'default'
);

$form['title']['alt-class'] = array(
  '#type'        => 'textfield',
  '#title'       => '',
  '#description' => 'Class name to use to identity element used for title',
  '#states'      => array(
    'invisible' => array(
      ':input[name="title-options"]' => array('value' => 'default'),
      ':input[name="title-options"]' => array('value' => 'alt-div')
    ),
    'visible' => array(
       ':input[name="title-options"]' => array('value' => 'alt-class')
    ),
  )
);

$form['title']['alt-div'] = array(
  '#type'        => 'textfield',
  '#title'       => '',
  '#description' => 'Div id to use to identity element used for title',
  '#states' => array(
    'invisible' => array(
      ':input[name="title-options"]' => array('value' => 'default'),
      ':input[name="title-options"]' => array('value' => 'alt-class')
    ),
    'visible' => array(
       ':input[name="title-options"]' => array('value' => 'alt-div')
    ),
  )
);

$form['title']['strip-from-title'] = array(
  '#type'          => 'checkbox',
  '#title'         => 'Strip from title',
  '#description'   => 'Strip text from title.',
  '#default_value' => 0,
);

$form['title']['strip-title-text'] = array(
  '#type'        => 'textfield',
  '#description' => 'Enter text to remove from title. This will affect all titles in the current batch.',
  '#states'      => array(
    'visible' => array(
        ':input[name="strip-from-title"]' => array('checked' => TRUE)
      ),
    'invisible' => array(
        ':input[name="strip-from-title"]' => array('checked' => FALSE)
      ),
  ),
);

$form['html_tags'] = array(
  '#type'        => 'fieldset',
  '#title'       => 'HTML Tags',
  '#collapsible' => TRUE,
  '#collapsed'   => FALSE
);

$form['html_tags']['body'] = array(
    '#type'          => 'checkbox',
    '#title'         => 'body',
    '#description'   => t('This will get all content between &lt;body&gt;&lt;/body&gt; tags. Uncheck this box to specify a div.'),
    '#default_value' => 0,
    '#weight'        => -10,
);

$form['html_tags']['strip-element-attributes'] = array(
    '#type'          => 'checkbox',
    '#title'         => t("Strip all element attributes"),
    '#default_value' => 1,
    '#weight'        => -8,
);

$form['html_tags']['allowed-element-attributes'] = array(
  '#type'        => 'textfield',
  '#description' => 'Enter attribute to preserve separate by commas (ex: href)',
  '#states'      => array(
    'visible' => array(
        ':input[name="strip-element-attributes"]' => array('checked' => FALSE)
      ),
    'invisible' => array(
        ':input[name="strip-element-attributes"]' => array('checked' => TRUE)
      ),
  ),
);

$form['html_tags']['non-body_tags'] = array(
    '#type'        => 'fieldset',
    '#title'       => t(''),
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
    '#states' => array(
      'visible' => array(
        ':input[name="body"]' => array('checked' => FALSE)
      ),
      'invisible' => array(
         ':input[name="body"]' => array('checked' => TRUE)
      )
    )
  );

  $form['html_tags']['non-body_tags']['div'] = array(
    '#type'        => 'textfield',
    '#title'       => 'Div id',
    '#description' => 'Extract only specified div id(s) (separated by spaces) and add to node body.',
    '#weight' => -9,
    '#default_value' => t('content'),
  );

// $form['html_tags']['non-body_tags']['class'] = array(
//     '#type' => 'textfield',
//     '#title' => 'Class name',
//     '#description' => ''
// );

  // $form['html_tags']['non-body_tags']['div-depth'] = array(
  //   '#type' => 'textfield',
  //   '#title' => t('Div depth'),
  //   '#description' => t('Depth to which to decend into the DOM, starting at 0')
  // );
/*
  $form['html_tags']['non-body_tags']['tags'] = array(
    '#type' => 'multiselect',
    '#title' => 'HTML tags',
    '#options' => get_html_tags(),
    '#default_value' => ''
  );
*/
/*
  $form['html_tags']['non-body_tags']['checkboxes'] = array(
    '#type' => 'checkboxes',
    '#options' => drupal_map_assoc(get_html_tags())
  );
  */

  $form['rewrite'] = array(
    '#type'        => 'fieldset',
    '#title'       => 'Rewrite',
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE
  );

  $form['rewrite']['rewrite_type'] = array(
    '#type'          => 'radios',
    '#title'         => 'Create a redirect to use on previous web server',
    '#options'       => drupal_map_assoc(['None', 'Apache', 'IIS']),
    '#default_value' => 'IIS',
  );

  $form['submit'] = array(
    '#type'   => 'submit',
    '#value'  => t('Submit'),
    '#weight' => 10
  );

  $form['date'] = array(
    '#type'        => 'fieldset',
    '#title'       => 'Post Date',
    '#description' => t('Drupal will use today\'s date by default for the posted date. Enter PHP regex string below to find the date in the imported content and use it for the post date. If you\'re regex is rusty, select a date format to search for below.'), 
    '#collapsible' => TRUE, 
    '#collapsed'   => FALSE
  );

  $form['date']['extract_date'] = array(
    '#type'    => 'textfield',
    '#title'   => t('Enter PHP regex to find date in content'),
    '#default_value' => '%[a-zA-Z]+\ [0-9]+\, [0-9]+%',
    '#description' => t('Default format: December 31, 1999'),
  );

  // $form['date']['set_separator'] = array(
  //   '#type'        => 'select',
  //   '#title'       => t('Select date separator used in original content'),
  //   '#options'     => drupal_map_assoc(['Select', '/', '-', '.']),
  //   '#description' => 'ex: 2015-12-31, 2015/12/31, 2015-12-31, etc.',
  // );

  // $form['date']['format'] = array(
  //   '#type'        => 'select',
  //   '#title'       => t('Select date format'),
  //   '#options'     => array(
  //     'Select' => 'Select',
  //     'F d, Y' => 'December 31, 1999',
  //     'Y-m-d'  => '1999-12-31',
  //     'm-d-Y'  => '12-31-1999',
  //     ),
  // );

  // $form['date']['set_date'] = array(
  //   '#type'          => 'textfield',
  //   '#title'         => t('Enter date to use in the following format: YYYY-MM-DD'),
  //   '#description'   => t('Set the date so that all posts in this batch have this date. Use format: YYYY-MM-DD, time and timezone are optional'),
  // );

  return $form;
}

function content_import_rules_form() {
  $form['download_rewrites'] = array(
    '#type'        => 'fieldset',
    '#title'       => 'Download Rewrite Rules',
    '#collapsible' => TRUE, 
    '#collapsed'   => FALSE
    );

  // $form['download_rewrites']['rules'] = array(
  //   '#type'        => 'select',
  //   '#title'       => t('Select rule to download'),
  //   '#options'     => get_rules(),
  //   '#description' => t('Select a rewrite to add to source filesystems configuration.')
  // );

  $form['download_rewrites']['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/*
 * Adminstrative form
 * /admin/config/content/content_import
 */
 function content_import_config_form() {
  $form['undo'] = array(
    '#type'        => 'fieldset',
    '#title'       => 'Undo',
    '#collapsible' => TRUE, 
    '#collapsed'   => FALSE
  );

  $form['undo']['batch_name'] = array(
    '#type'        => 'select',
    '#title'       => t('Select batch(es) to delete'),
    '#options'     => drupal_map_assoc(get_batch_names()),
    '#description' => t('Permanently delete nodes in batch(es). This cannot be undone.')
  );

  $form['undo']['submit_undo'] = array(
    '#type'  => 'submit',
    '#value' => t('Submit'),
    '#submit' => array('delete_batch_nodes'),
  );

  $form['rewrites'] = array(
    '#type'        => 'fieldset',
    '#title'       => 'Rewrites',
    '#collapsible' => TRUE, 
    '#collapsed'   => FALSE
  );
  $form['rewrites']['rewrite_name'] = array(
    '#type'        => 'select',
    '#title'       => t('Select batch to download its rewrites'),
    //'#options'     => get_rewrites(),
    '#options'     =>drupal_map_assoc(get_batch_names()),
    '#description' => t('Downloads a text file of rewrites.'),
    '#default_value' => null,
  );

  $form['rewrites']['download'] = array(
    '#title'  => 'Download Rewrites',
    '#description' => '',
    '#type'   => 'submit',
    '#value'  => t('Download'),
    '#submit' => array('download_rewrites_submit'),
  );

    return $form;
}
/*
 * End administrative form
 */
