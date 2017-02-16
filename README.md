# Finding Aids Auto Link

This repo contains code for two different systems: Omeka and Drupal.

The Omeka plugin sends a request to the Drupal module whenever an *public* item is saved in Omeka.

The Drupal endpoint then searches its finding aids with the metadata provided by Omeka, and attempts to link the appropriate text in the finding aid to the item in Omeka. Drupal will only link the text if it finds a unique match.

![Screencast of what happens when these two systems work together](/images/screencast.gif)

## Installation

Clone this repository.

Add the "FindingAids" directory in this repository to your Omeka "plugins" directory.

Add the "finding_aids" directory in this repository to your Drupal "modules" directory.

Enable the plugin and module in the respective system.


## Configuration

1.) In Drupal, configure settings at /admin/config/finding-aid. You will need to know the IP address for your Omeka server, the base URL for your Omeka server, which Content Type is used to store your finding aids in Drupal, and you will need to create a special key via the key module.

2.) In Omeka's configuration for the FindingAids plugin (at admin/plugins/config?name=FindingAids) provide the URL to the Drupal endpoint for both your production and test systems, the secret key created in step #1 above, and select which element stores the Finding Aid URL in the Omeka Item Record.

------

Developed by [Kent State University Libraries](http://www.library.kent.edu).
