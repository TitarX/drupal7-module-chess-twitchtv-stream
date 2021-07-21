<?php

function chess_twitchtv_stream_access_by_role($access_roles) {
  global $user;

  foreach ($access_roles as $access_role) {
    if (in_array($access_role, $user->roles)) {
      return TRUE;
    }
  }

  return FALSE;
}
