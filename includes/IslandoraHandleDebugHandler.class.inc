<?php

/**
 * Debug handler for when there's no Handle server to interact with.
 */
class IslandoraHandleDebugHandler extends IslandoraHandleHandleHandler {

  /**
   * Creates a Handle in the database for debugging.
   *
   * @param AbstractObject $object
   *   The AbstractObject having a handle minted for it.
   *
   * @return bool
   *   TRUE on creation; FALSE otherwise.
   */
  public function createHandle(AbstractObject $object) {
    $suffix = $this->constructSuffix($object);
    db_insert('islandora_handle_debug_handles')
      ->fields(array(
        'suffix' => $suffix,
        'location' => $this->constructTargetUrl($suffix),
      ))
      ->execute();
    return TRUE;
  }

  /**
   * Gets an existing Handle's location from the database for debugging.
   *
   * @param string|AbstractObject $handle
   *   A Handle that we are updating in prefix/suffix form or an AbstractObject
   *   that is being updated. In the AbstractObject case a Handle URL will be
   *   constructed to compare.
   *
   * @return string|bool
   *   The location of the Handle if it exists; FALSE otherwise.
   */
  public function readHandle($handle) {
    // Need to munge just the suffix out for debug case.
    return db_select('islandora_handle_debug_handles', 'd')
      ->fields('d', array('location'))
      ->condition('suffix', $handle->id, '=')
      ->execute()
      ->fetchField();
  }

  /**
   * Updates a Handle's location in the database for debugging.
   *
   * @param string|AbstractObject $handle
   *   A Handle that we are updating in prefix/suffix form or an AbstractObject
   *   that is being updated. In the AbstractObject case a Handle URL will be
   *   constructed to compare.
   * @param string $target
   *   The target URL the Handle is being updated to point to.
   *
   * @return bool
   *   TRUE if successful; FALSE otherwise.
   */
  public function updateHandle($handle, $target) {
    $prefix = variable_get('islandora_handle_server_prefix', '1234567');
    $suffix = str_replace("$prefix/", '', $handle);
    db_merge('islandora_handle_debug_handles')
      ->fields(array(
        'location' => $target,
        'suffix' => $suffix,
      ))
      ->condition('suffix', $suffix)
      ->execute();
    return TRUE;
  }

  /**
   * Deletes a Handle from the database for debugging.
   *
   * @param string|AbstractObject $handle
   *   A Handle that we are deleting in prefix/suffix form or an AbstractObject
   *   that is being deleted. In the AbstractObject case a Handle URL will be
   *   constructed to compare.
   *
   * @return bool
   *   TRUE if successful; FALSE otherwise.
   */
  public function deleteHandle($handle) {
    db_delete('islandora_handle_debug_handles')
      ->condition('suffix', $handle->id)
      ->execute();
    return TRUE;
  }

}
