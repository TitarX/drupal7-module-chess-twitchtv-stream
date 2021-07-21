<?php

module_load_include(
  'inc',
  'chess_twitchtv_stream',
  'includes/chess_twitchtv_stream_api'
);

function chess_twitchtv_stream_streams_page_contents() {
  drupal_add_css(
    drupal_get_path(
      'module',
      'chess_twitchtv_stream'
    ) . '/css/chess_twitchtv_stream_extra.css'
  );
  drupal_add_js(
    drupal_get_path(
      'module',
      'chess_twitchtv_stream'
    ) . '/js/chess_twitchtv_stream_streams_loading.js'
  );

  drupal_set_title("Шахматные онлайн трансляции");

  $keywords_element = [
    '#tag' => 'meta',
    '#attributes' => [
      'name' => 'keywords',
      'content' => 'шахматные видеотрансляции, онлайн трансляции шахматы, шахматный стрим',
    ],
  ];
  drupal_add_html_head($keywords_element, 'Keywords');

  $description_element = [
    '#tag' => 'meta',
    '#attributes' => [
      'name' => 'description',
      'content' => 'Живые видеотрансляции по шахматам, любителей и профессионалов игры.',
    ],
  ];
  drupal_add_html_head($description_element, 'Description');

  $text = '';
  $ad_content_first = '';
  $ad_content_second = '';
  $ad_content_third = '';
  $ad_content_fourth = '';

  $query = db_select('chess_twitchtv_stream');
  $query->fields(
    'chess_twitchtv_stream',
    [
      'text',
      'ad_content_first',
      'ad_content_second',
      'ad_content_third',
      'ad_content_fourth',
    ]
  );
  $chess_twitchtv_stream_results = $query->execute()->fetchAll();

  $chess_twitchtv_stream_results = array_shift($chess_twitchtv_stream_results);
  if ($chess_twitchtv_stream_results) {
    $text = (isset($chess_twitchtv_stream_results->text) ? trim(
      $chess_twitchtv_stream_results->text
    ) : '');
    $ad_content_first = (isset($chess_twitchtv_stream_results->ad_content_first) ? trim(
      $chess_twitchtv_stream_results->ad_content_first
    ) : '');
    $ad_content_second = (isset($chess_twitchtv_stream_results->ad_content_second) ? trim(
      $chess_twitchtv_stream_results->ad_content_second
    ) : '');
    $ad_content_third = (isset($chess_twitchtv_stream_results->ad_content_third) ? trim(
      $chess_twitchtv_stream_results->ad_content_third
    ) : '');
    $ad_content_fourth = (isset($chess_twitchtv_stream_results->ad_content_fourth) ? trim(
      $chess_twitchtv_stream_results->ad_content_fourth
    ) : '');
  }

  $chess_twitchtv_stream_streams_markup = '';

  if (!empty($text)) {
    $chess_twitchtv_stream_streams_markup .= '<div id="streams-text-wrapper">';
    $chess_twitchtv_stream_streams_markup .= $text;
    $chess_twitchtv_stream_streams_markup .= '</div>';
  }

  $chess_twitchtv_stream_streams_markup .= '<div class="ad-content-top-wrapper streams-block">';
  if (!empty($ad_content_first)) {
    $chess_twitchtv_stream_streams_markup .= '<div id="ad-content-first-wrapper">';
    $chess_twitchtv_stream_streams_markup .= $ad_content_first;
    $chess_twitchtv_stream_streams_markup .= '</div>';
  }

  if (!empty($ad_content_second)) {
    $chess_twitchtv_stream_streams_markup .= '<div id="ad-content-second-wrapper">';
    $chess_twitchtv_stream_streams_markup .= $ad_content_second;
    $chess_twitchtv_stream_streams_markup .= '</div>';
  }
  $chess_twitchtv_stream_streams_markup .= '</div>';

  $chess_twitchtv_stream_streams_markup .= '<div id="magnify-wrapper" class="streams-block">';
  $chess_twitchtv_stream_streams_markup .= file_get_contents(
    drupal_get_path('module', 'chess_twitchtv_stream') . '/includes/magnify.inc'
  );
  $chess_twitchtv_stream_streams_markup .= '</div>';

  $chess_twitchtv_stream_streams_markup .= '<div id="empty-message-wrapper" class="streams-block" style="display: none;">';
  $chess_twitchtv_stream_streams_markup .= '<br />';
  $chess_twitchtv_stream_streams_markup .= '<strong>Извините, в данное время все трансляции OFFLINE.</strong>';
  $chess_twitchtv_stream_streams_markup .= '<br />';
  $chess_twitchtv_stream_streams_markup .= '</div>';

  $chess_twitchtv_stream_streams_markup .= '<div id="streams-content">';
  $chess_twitchtv_stream_streams_markup .= '</div>';

  $chess_twitchtv_stream_streams_markup .= '<div class="ad-content-bottom-wrapper streams-block">';
  if (!empty($ad_content_third)) {
    $chess_twitchtv_stream_streams_markup .= '<div id="ad-content-third-wrapper">';
    $chess_twitchtv_stream_streams_markup .= $ad_content_third;
    $chess_twitchtv_stream_streams_markup .= '</div>';
  }

  if (!empty($ad_content_fourth)) {
    $chess_twitchtv_stream_streams_markup .= '<div id="ad-content-fourth-wrapper">';
    $chess_twitchtv_stream_streams_markup .= $ad_content_fourth;
    $chess_twitchtv_stream_streams_markup .= '</div>';
  }
  $chess_twitchtv_stream_streams_markup .= '</div>';

  $output['chess_twitchtv_stream_streams_markup'] = [
    '#type' => 'markup',
    '#markup' => $chess_twitchtv_stream_streams_markup,
  ];

  return $output;
}

function chess_twitchtv_stream_ajax_get_ajax($stream_id) {
  $output = ['next' => 'end', 'markup' => ''];

  $client_id = '';
  $client_secret = '';
  $token = '';

  $query = db_select('chess_twitchtv_stream');
  $query->fields(
    'chess_twitchtv_stream',
    [
      'client_id',
      'client_secret',
      'token',
      'text',
      'ad_content_first',
      'ad_content_second',
      'ad_content_third',
      'ad_content_fourth',
    ]
  );
  $chess_twitchtv_stream_results = $query->execute()->fetchAll();

  $chess_twitchtv_stream_results = array_shift($chess_twitchtv_stream_results);
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

  if (empty($client_id) || empty($client_secret)) {
    return json_encode($output, JSON_UNESCAPED_UNICODE);
  }
  else {
    $query = '';
    if ($stream_id == 'begin') {
      $query = db_query(
        "SELECT id, user_id, user_name, stream_name, status FROM chess_twitchtv_stream_streams WHERE id > -1 ORDER BY id ASC LIMIT 1"
      );
    }
    else {
      $query = db_query(
        "SELECT id, user_id, user_name, stream_name, status FROM chess_twitchtv_stream_streams WHERE id LIKE :stream_id",
        [':stream_id' => $stream_id]
      );
    }
    $currentStreamResult = $query->fetchAll();

    $currentStreamResult = array_shift($currentStreamResult);
    if ($currentStreamResult) {
      // Проверяем доступность стрима
      $is_break_chanel = TRUE;
      if ($currentStreamResult->status == '1') {
        $user_id = trim($currentStreamResult->user_id);
        if (empty($user_id)) {
          $user_id = chess_twitchtv_stream_api_get_user_id(
            $client_id,
            $client_secret,
            $token,
            $currentStreamResult->stream_name
          );

          if ($user_id === FALSE) {
            return json_encode($output, JSON_UNESCAPED_UNICODE);
          }
          else {
            if ($user_id !== NULL) {
              $query = db_update('chess_twitchtv_stream_streams');

              $query_fields = [
                'user_id' => $user_id,
              ];

              $query->fields($query_fields);
              $query->condition('id', $currentStreamResult->id);
              $query->execute();
            }
          }
        }

        if (!empty($user_id)) {
          $sream_id = chess_twitchtv_stream_api_get_stream_id(
            $client_id,
            $client_secret,
            $token,
            $user_id
          );
          if ($sream_id === FALSE) {
            return json_encode($output, JSON_UNESCAPED_UNICODE);
          }
          else {
            if ($sream_id !== NULL) {
              $is_break_chanel = FALSE;
            }
          }
        }
      }

      if ($is_break_chanel) {
        $output['markup'] = 'break';
      }
      else {
        $output['markup'] = '';
        $output['markup'] .= '<div class="stream-wrapper streams-block">';
        $output['markup'] .= '<h3 class="stream-header">Онлайн трансляция пользователя ' . $currentStreamResult->user_name . '</h3>';
        $output['markup'] .= (empty($stream_json_array['streams'][0]['channel']['status']) ? '' : ('<p class="stream-description">' . $stream_json_array['streams'][0]['channel']['status'] . '</p>'));
        // $output['markup'] .= '<noindex>';
        $output['markup'] .= '<iframe src="https://player.twitch.tv/?channel=' . $currentStreamResult->stream_name . '&parent=chess4all.club&autoplay=false&muted=false" name="' . $currentStreamResult->user_name . '" allowfullscreen="true" frameborder="0" scrolling="no" width="940" height="573">';
        $output['markup'] .= '<meta name="robots" content="noindex, nofollow" />';
        $output['markup'] .= '</iframe>';
        // $output['markup'] .= '</noindex>';
        $output['markup'] .= '</div>';
      }

      $query = db_query(
        "SELECT id FROM chess_twitchtv_stream_streams WHERE id > :currentId ORDER BY id ASC LIMIT 1",
        [':currentId' => $currentStreamResult->id]
      );
      $nextIdResult = $query->fetchAll();

      $nextIdResult = array_shift($nextIdResult);
      if ($nextIdResult) {
        $output['next'] = $nextIdResult->id;
      }
      else {
        $output['next'] = 'end';
      }

      return json_encode($output, JSON_UNESCAPED_UNICODE);
    }
    else {
      return json_encode($output, JSON_UNESCAPED_UNICODE);
    }
  }
}

function chess_twitchtv_stream_ajax_callback($page_callback_result) {
  print $page_callback_result;
}
