<?php

module_load_include(
  'inc',
  'chess_twitchtv_stream',
  'includes/chess_twitchtv_stream_api'
);

function chess_twitchtv_stream_availability_stream_checking() {
  $key = '7nae0d9y2iXX29CIuIp4Iq280f0gLlO0';

  $parameters = drupal_get_query_parameters();

  if (isset($parameters['key']) && $parameters['key'] == $key) {
    $query = db_select('chess_twitchtv_stream');
    $query->fields(
      'chess_twitchtv_stream',
      ['client_id', 'client_secret', 'token']
    );
    $chess_twitchtv_stream_results = $query->execute()->fetchAll();

    $chess_twitchtv_stream_results = array_shift(
      $chess_twitchtv_stream_results
    );
    if ($chess_twitchtv_stream_results) {
      $client_id = (isset($chess_twitchtv_stream_results->client_id) ? trim(
        $chess_twitchtv_stream_results->client_id
      ) : '');
      $client_secret = (isset($chess_twitchtv_stream_results->client_secret) ? trim(
        $chess_twitchtv_stream_results->client_secret
      ) : '');
      $token = (isset($chess_twitchtv_stream_results->token) ? trim(
        $chess_twitchtv_stream_results->token
      ) : '');
    }

    if (!empty($client_id) && !empty($client_secret)) {
      $query = db_select('chess_twitchtv_stream_streams');
      $query->fields(
        'chess_twitchtv_stream_streams',
        [
          'id',
          'user_id',
          'stream_name',
          'last_availability_time',
        ]
      );
      $chess_twitchtv_stream_streams_results = $query->execute()->fetchAll();

      $streams_for_deleteting = [];

      foreach ($chess_twitchtv_stream_streams_results as $chess_twitchtv_stream_streams_result) {
        $item_id = (isset($chess_twitchtv_stream_streams_result->id) ? trim(
          $chess_twitchtv_stream_streams_result->id
        ) : '');
        $user_id = (isset($chess_twitchtv_stream_streams_result->user_id) ? trim(
          $chess_twitchtv_stream_streams_result->user_id
        ) : '');
        $stream_name = (isset($chess_twitchtv_stream_streams_result->stream_name) ? trim(
          $chess_twitchtv_stream_streams_result->stream_name
        ) : '');
        $last_availability_time = (isset($chess_twitchtv_stream_streams_result->last_availability_time) ? trim(
          $chess_twitchtv_stream_streams_result->last_availability_time
        ) : '');

        if (!empty($item_id) && !empty($stream_name)) {
          $is_stream_outdate = FALSE;
          if (!empty($last_availability_time)) {
            if (!is_int($last_availability_time)) {
              $last_availability_time = intval($last_availability_time);
            }

            if ($last_availability_time > 1) {
              $now_time = strtotime("now");
              $up_1_months = strtotime("+1 months");
              $time_1_months = $up_1_months - $now_time;
              $up_1_months_from_last_availability_time = $last_availability_time + $time_1_months;

              if ($now_time > $up_1_months_from_last_availability_time) {
                $is_stream_outdate = TRUE;
                array_push($streams_for_deleteting, $item_id);
              }
            }
          }

          if (!$is_stream_outdate) {
            $new_stream_status = '1';

            $user_id = trim($user_id);
            if (empty($user_id)) {
              $user_id = chess_twitchtv_stream_api_get_user_id(
                $client_id,
                $client_secret,
                $token,
                $stream_name
              );

              if ($user_id === FALSE) {
                die();
              }
              else {
                if ($user_id === NULL) {
                  $rand_var = mt_rand(1000000, 4000000);
                  usleep($rand_var);

                  continue;
                }
              }
            }

            $sream_id = chess_twitchtv_stream_api_get_stream_id(
              $client_id,
              $client_secret,
              $token,
              $user_id
            );
            if ($sream_id === FALSE) {
              die();
            }
            else {
              if ($sream_id === NULL) {
                $new_stream_status = '0';
              }
            }

            $query = db_update('chess_twitchtv_stream_streams');

            $query_fields = [
              'status' => $new_stream_status,
              'user_id' => $user_id,
            ];

            if ($new_stream_status == '1') {
              $current_time = time();
              $query_fields['last_availability_time'] = $current_time;
            }

            $query->fields($query_fields);
            $query->condition('id', $item_id);
            $query->execute();

            print '|';
            print $item_id;
            print '|';

            $rand_var = mt_rand(1000000, 4000000);
            usleep($rand_var);
          }
        }
      }

      foreach ($streams_for_deleteting as $stream_for_deleteting) {
        $query = db_delete('chess_twitchtv_stream_streams');
        $query->condition('id', $stream_for_deleteting);
        $query->execute();
      }
    }
  }

  print '|Done|';
}
