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
