<?php

/**
 * @file
 * Content Panel ctools content type.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Content Panel'),
  'content_types' => 'content_panel',
  'description' => t('Render a content panel based on the selected options.'),
  'render callback' => 'content_panel_render',
  'edit form' => 'content_panel_edit_form',
  'defaults' => array(
    'entity_type' => '',
    'bundle' => '',
    'view_mode' => '',
    'uuid' => '',
  ),
  'category' => t('Content Panels'),
);

/**
 * Returns an edit form for custom type settings.
 */
function content_panel_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  $info = entity_get_info();
  $entity_type = !empty($conf['entity_type']) ? $conf['entity_type'] : 'node';

  // @TODO: AJAX FOR CHANGING ENTITY TYPE.

  $form['entity_type'] = array(
    '#title' => t('Entity Type'),
    '#type' => 'select',
    '#options' => content_panel_get_entity_options($info),
    '#default_value' => $entity_type,
    '#required' => TRUE,
  );

  $form['bundle'] = array(
    '#title' => t('Bundle'),
    '#type' => 'select',
    '#options' => content_panel_get_bundle_options($info, $entity_type),
    '#default_value' => isset($conf['bundle']) ? $conf['bundle'] : '',
    '#required' => TRUE,
  );

  $form['view_mode'] = array(
    '#title' => t('View Mode'),
    '#type' => 'select',
    '#options' => content_panel_get_view_mode_options($info, $entity_type),
    '#default_value' => isset($conf['view_mode']) ? $conf['view_mode'] : '',
    '#required' => TRUE,
  );

  return $form;
}

/**
 * Submit handler for the custom type settings form.
 */
function content_panel_edit_form_submit($form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }

  // Append uuid to config to use it in render.
  $display = $form_state['display'];
  if (empty($form_state['conf']['uuid'])) {
    $form_state['conf']['uuid'] = ctools_uuid_generate();
  }
  $record = array(
    'uuid' => $form_state['conf']['uuid'],
    'entity_type' => $form_state['conf']['entity_type'],
    'bundle' => $form_state['conf']['bundle'],
  );
  drupal_write_record('content_panels_uuid_entity', $record);
}

/**
 * Get entity type options.
 */
function content_panel_get_entity_options($info) {
  $options = array();
  foreach ($info as $entity_type_name => $entity_type_info) {
    // We don't want comments and vocabularies.
    if ($entity_type_name !== 'comment' && $entity_type_name !== 'taxonomy_vocabulary') {
      $options[$entity_type_name] = $entity_type_info['label'];
    }
  }
  return $options;
}

/**
 * Get entity type bundle options.
 */
function content_panel_get_bundle_options($info, $entity_type) {
  $options = array();
  if (isset($info[$entity_type])) {
    foreach ($info[$entity_type]['bundles'] as $bundle_name => $bundle_info) {
      $options[$bundle_name] = $bundle_info['label'];
    }
  }
  return $options;
}

/**
 * Get entity type view_mode options.
 */
function content_panel_get_view_mode_options($info, $entity_type) {
  $options = array();
  if (isset($info[$entity_type])) {
    foreach ($info[$entity_type]['view modes'] as $view_mode_name => $view_mode_info) {
      $options[$view_mode_name] = $view_mode_info['label'];
    }
  }
  return $options;
}

/**
 * Render content type.
 */
function content_panel_render($subtype, $conf, $panel_args, $context) {
  $block = new stdClass();
  $block->title   = '';
  $block->content = array();

  $uuid = $conf['uuid'];
  $entity_type = $conf['entity_type'];
  $view_mode = $conf['view_mode'];

  $entity_id = db_query('SELECT entity_id FROM {content_panels_uuid_entity} WHERE uuid = :uuid', array(
    ':uuid' => $uuid,
  ))->fetchField();

  if ($entity_id) {
    $entities = entity_load($entity_type, array($entity_id));
    $block->content['entity'] = entity_view($entity_type, $entities, $view_mode);
  }
  else {
    $bundle = $conf['bundle'];
    // No entity, show create form if allowed.
    $path = content_panel_get_entity_add_path($entity_type, $bundle);
    if (drupal_valid_path($path)) {
      $block->content['add_link'] = array(
        '#theme' => 'link',
        '#text' => t('Add entity'),
        '#path' => $path,
        '#options' => array(
          'attributes' => array(),
          'html' => FALSE,
          'query' => array(
            'content_panels_uuid' => $uuid,
          ),
        ),
      );
    }
  }

  return $block;
}
