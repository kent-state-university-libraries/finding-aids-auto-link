# Finding Aids Auto Link

This repo contains code for two different systems: Omeka and Drupal.

The Omeka plugin sends a request to the Drupal module whenever an *public* item is saved in Omeka.

The Drupal endpoint then searches its finding aids with the metadata provided by Omeka, and attempts to link the appropriate text in the finding aid to the item in Omeka. Drupal will only link the text if it finds a unique match.


## Installation

Clone this repository.

Add the "FindingAids" directory in this repository to your Omeka "plugins" directory.

Add the "finding_aids" directory in this repository to your Drupal "modules" directory.

Enable the plugin and module in both systems.


## Configuration

Provide the Omeka plugin with the endpoint to both your production and test systems. Also select which element stores the Finding Aid URL in the Omeka Item Record.

------

Developed by [Kent State University Libraries](http://www.library.kent.edu).
