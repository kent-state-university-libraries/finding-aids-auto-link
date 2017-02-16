<style type="text/css">
  input[type="url"] {width: 100%;}
</style>

<div class="field">
    <div class="two columns alpha">
        <label for="finding_aids_production_endpoint"><?php echo __('Production Endpoint');?></label>
    </div>
    <div class="inputs five columns omega">
        <input type="url" name="finding_aids_production_endpoint" value="<?php echo get_option("finding_aids_production_endpoint"); ?>" />
    </div>
</div>

<div class="field">
    <div class="two columns alpha">
        <label for="finding_aids_test_endpoint"><?php echo __('Test Endpoint (optional)');?></label>
    </div>
    <div class="inputs five columns omega">
        <input type="url" name="finding_aids_test_endpoint" value="<?php echo get_option("finding_aids_test_endpoint"); ?>" />
        <p>This endpoint will be used if this instance of Omeka is running in test. If you don't have a test instance, you can leave this blank.</p>
    </div>
</div>

<div class="field">
    <div class="two columns alpha">
        <label for="finding_aids_secret_key"><?php echo __('Secret Key');?></label>
    </div>
    <div class="inputs five columns omega">
        <input type="text" name="finding_aids_secret_key" value="<?php echo get_option("finding_aids_secret_key"); ?>" />
        <p>The key value that was set on your endpoint.</p>
    </div>
</div>

<div class="field">
    <div class="two columns alpha">
        <label for="finding_aids_element"><?php echo __('Element');?></label>
    </div>
    <div class="inputs five columns omega">
        <?php echo get_view()->formSelect('finding_aids_element', get_option('finding_aids_element'), array(), $options); ?>
        <p>This is the element that stores the full URL of the Finding Aid</p>
    </div>
</div>
