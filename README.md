content_import

Drupal Module to rip site content and insert it into node.

**Current Features:**

Process URL(s) and create node(s) and add the content of the body tag for that page to the newly created node.

Assign content type to the node



**TODO:**


get_url(): check for empty array and return message

attach_files(): only process files of specific mime-types

~~process HTML Tags fieldset, only import content of those tags)~~

~~add batch rollback to delete nodes created by that batch~~

add rule to check if node being deleted is part of batch, if so, give option to delete single node from batch or entire batch

add option to modify attribute of nodes in batch (content type, owner, etc.)

add option to modfy attributes of batch (name, date, etc.)

~~Make whole non-body tags invisible when body checkbox is checked~~

Import mime types from URL and save as media and as file to the content type

