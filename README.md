<<<<<<< HEAD
=======
<<<<<<< HEAD
content_import
==============

Drupal Module to rip site content and insert it into node.

Current Features:

Process URL(s) and create node(s) and add the content of the body tag for that page to the newly created node.

Assign content type to the node



TODO:


get_url(): check for empty array and return message

attach_files(): only process files of specific mime-types

process HTML Tags fieldset, only import content of those tags

add batch rollback to delete nodes created by that batch

add rule to check if node being deleted is part of batch, if so, give option to delete single node from batch or entire batch

add option to modify attribute of nodes in batch (content type, owner, etc.)

add option to modfy attributes of batch (name, date, etc.)

Make whole non-body tags invisible when body checkbox is checked

Import mime types from URL and save as media and as file to the content type


=======
>>>>>>> 508e0217e16e1bf1364182ff5c56399433c34bae
link_import
===========
Link Import is a working title, since it was with that original purpose in mind that I started writing the module.

In a nutshell, link_import is a Drupal module for importing content from a non-Drupal site into a Drupal site. Instead of copying pages, links, and files by hand, just give the tools a list of URLs, set some options such as importing file types, copying the HTML body of a page into the body of a new node, setting the content type and taxonomy for that node,
etctera.

There may be others out there, but this was something I needed and gave me motivation to learn Drupal module development.
<<<<<<< HEAD
=======
>>>>>>> master
>>>>>>> 508e0217e16e1bf1364182ff5c56399433c34bae
