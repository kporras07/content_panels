# Content Panels

Contents of this file
=====================

* Introduction
* Installation
* Configuration
* Maintainers


## Introduction

This module provides developer the possibility of placing unexisting entity view modes in panels (and related) and create
those entities later without marking features as overriden. Keep in mind, the entity wouldn't be exported in panels config
but there's an internal mapping done between the content panel placed and the later created entity.

### How does this work?

When added a new content panel to a page (or similar), you get a config form where you select entity type, entity bundle
and view mode. This settings (alongside an internal id) get exported with panel configuration. When viewing that page, if you
have the right permissions, you get a "Create Entity" link. Once created, that entity gets associated to the panel where
it's coming from through a db table maintained by this module.

## Installation

Install as you would normally install a contributed drupal module.
See: https://drupal.org/documentation/install/modules-themes/modules-7 for further information.

## Configuration

Place a Content Panel block into a page, select the right settings for your needs and create the entity from the page view
(real view, not the administrative one).

## Maintainers

* Kevin Porras Zumbado (kporras07) - https://drupal.org/kporras07
