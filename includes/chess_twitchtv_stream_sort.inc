<?php

function chess_twitchtv_stream_sort_by_time_desc($val1, $val2) {
  if ($val1->last_availability_time == $val2->last_availability_time) {
    return 0;
  }

  return ($val1->last_availability_time < $val2->last_availability_time) ? 1 : -1;
}
