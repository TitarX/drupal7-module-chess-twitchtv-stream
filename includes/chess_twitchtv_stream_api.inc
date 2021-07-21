<?php

// Возвращает:
// Токен - при успешном выполнении
// NULL - если не удалось получить новый токен
function chess_twitchtv_stream_api_get_token($client_id, $client_secret) {
  $url = 'https://id.twitch.tv/oauth2/token';
  $fields = "client_id=$client_id&client_secret=$client_secret&grant_type=client_credentials";
  $curl_exec_result = chess_twitchtv_stream_api_curl_exec_post($url, $fields);

  // Array ( [access_token] => 1nqabdylie2u1qpncnk14s20zfigct [expires_in] => 5524798 [token_type] => bearer )
  $json_decode_result = json_decode($curl_exec_result, TRUE);
  if (json_last_error(
    ) === JSON_ERROR_NONE && !empty($json_decode_result['access_token'])) {
    $query = db_update('chess_twitchtv_stream');
    $query->fields(
      [
        'token' => $json_decode_result['access_token'],
      ]
    );
    $exe_result = $query->execute();

    if ($exe_result === NULL) {
      $message = 'Не удалось записать новый токен в базу данных';
      watchdog('chess_twitchtv_stream', $message, [], WATCHDOG_ERROR);
    }

    return $json_decode_result['access_token'];
  }
  else {
    $message = 'Не удалось получить новый токен';
    watchdog('chess_twitchtv_stream', $message, [], WATCHDOG_ERROR);

    return NULL;
  }
}

// Возвращает:
// Идентификатор пользователя - при успешном выполнении
// NULL - если не удалось получить идентификатор пользователя
// FALSE - если токен недействительный и не удалось получить новый токен, выполнение должно быть прервано
function chess_twitchtv_stream_api_get_user_id(
  $client_id,
  $client_secret,
  $token,
  $stream_name
) {
  $url = "https://api.twitch.tv/helix/users?login=$stream_name";
  $curl_exec_result = chess_twitchtv_stream_api_curl_exec_get(
    $url,
    $client_id,
    $token
  );

  // Array ( [data] => Array ( [0] => Array ( [id] => 7601562 [login] => chess [display_name] => Chess [type] => [broadcaster_type] => partner [description] => Chess.com is the world's #1 chess site with millions of chess players and enthusiasts. Sign up - it's FREE! [profile_image_url] => https://static-cdn.jtvnw.net/jtv_user_pictures/320d61dc-5202-44ad-a203-8a8d7a9cbfa9-profile_image-300x300.png [offline_image_url] => https://static-cdn.jtvnw.net/jtv_user_pictures/dc4d2d24-6bff-4e67-9a4a-03b0c91a26d1-channel_offline_image-1920x1080.png [view_count] => 202343259 [created_at] => 2009-08-06T19:59:14.118428Z ) ) )
  // Или, если токен недействителен
  // Array ( [error] => Unauthorized [status] => 401 [message] => Invalid OAuth token )
  $json_decode_result = json_decode($curl_exec_result, TRUE);
  $json_decode_result = chess_twitchtv_stream_api_json_decode_result_check(
    $json_decode_result,
    $url,
    $client_id,
    $client_secret
  );
  if ($json_decode_result === FALSE) {
    return FALSE;
  }
  else {
    if ($json_decode_result === NULL) {
      return NULL;
    }
    else {
      if (!empty($json_decode_result['data'][0]['id'])) {
        return $json_decode_result['data'][0]['id'];
      }
      else {
        return NULL;
      }
    }
  }
}

// Возвращает:
// Идентификатор пользователя - при успешном выполнении
// NULL - если не удалось получить идентификатор пользователя
// FALSE - если токен недействительный и не удалось получить новый токен, выполнение должно быть прервано
function chess_twitchtv_stream_api_get_stream_id(
  $client_id,
  $client_secret,
  $token,
  $user_id
) {
  $url = "https://api.twitch.tv/kraken/streams/$user_id";
  $curl_exec_result = chess_twitchtv_stream_api_curl_exec_get(
    $url,
    $client_id,
    $token
  );

  // Array ( [stream] => Array ( [_id] => 40514467565 [game] => Chess [broadcast_platform] => live [community_id] => [community_ids] => Array ( ) [viewers] => 34895 [video_height] => 1080 [average_fps] => 60 [delay] => 0 [created_at] => 2021-02-09T15:39:46Z [is_playlist] => [stream_type] => live [preview] => Array ( [small] => https://static-cdn.jtvnw.net/previews-ttv/live_user_gmhikaru-80x45.jpg [medium] => https://static-cdn.jtvnw.net/previews-ttv/live_user_gmhikaru-320x180.jpg [large] => https://static-cdn.jtvnw.net/previews-ttv/live_user_gmhikaru-640x360.jpg [template] => https://static-cdn.jtvnw.net/previews-ttv/live_user_gmhikaru-{width}x{height}.jpg ) [channel] => Array ( [mature] => [status] => !guest GM ALIREZA FIROUZJA | Opera Euro Rapid Quarterfinals Day 1 | !format !today [broadcaster_language] => en [broadcaster_software] => [display_name] => GMHikaru [game] => Chess [language] => en [_id] => 103268673 [name] => gmhikaru [created_at] => 2015-09-29T10:35:23.386423Z [updated_at] => 2021-02-09T17:33:54.086089Z [partner] => 1 [logo] => https://static-cdn.jtvnw.net/jtv_user_pictures/cd921fcd-53fc-4b1d-be23-0787bc36b756-profile_image-300x300.png [video_banner] => https://static-cdn.jtvnw.net/jtv_user_pictures/cc6af6f3-acb3-4a32-8223-13de1d2aadd1-channel_offline_image-1920x1080.png [profile_banner] => https://static-cdn.jtvnw.net/jtv_user_pictures/f7d1baa5-3d5e-465d-954c-805f2d95a88a-profile_banner-480.png [profile_banner_background_color] => [url] => https://www.twitch.tv/gmhikaru [views] => 64153266 [followers] => 970722 [broadcaster_type] => [description] => Grandmaster Hikaru Nakamura, 5-time United States Chess Champion [private_video] => [privacy_options_enabled] => ) ) )
  // Или, если трансляция отключена:
  // Array ( [stream] => )
  $json_decode_result = json_decode($curl_exec_result, TRUE);
  $json_decode_result = chess_twitchtv_stream_api_json_decode_result_check(
    $json_decode_result,
    $url,
    $client_id,
    $client_secret
  );
  if ($json_decode_result === FALSE) {
    return FALSE;
  }
  else {
    if ($json_decode_result === NULL) {
      return NULL;
    }
    else {
      if (!empty($json_decode_result['stream']['_id'])) {
        return $json_decode_result['stream']['_id'];
      }
      else {
        return NULL;
      }
    }
  }
}

function chess_twitchtv_stream_api_curl_exec_get($url, $client_id, $token) {
  $curl = curl_init($url);

  curl_setopt($curl, CURLOPT_RETURNTRANSFER, TRUE);
  curl_setopt(
    $curl,
    CURLOPT_HTTPHEADER,
    [
      'Accept: application/vnd.twitchtv.v5+json',
      "Client-ID: $client_id",
      "Authorization: Bearer $token",
    ]
  );

  $curl_exec_result = curl_exec($curl);
  curl_close($curl);

  return $curl_exec_result;
}

function chess_twitchtv_stream_api_curl_exec_post($url, $fields) {
  $curl = curl_init();

  curl_setopt($curl, CURLOPT_RETURNTRANSFER, TRUE);
  curl_setopt($curl, CURLOPT_URL, $url);
  curl_setopt($curl, CURLOPT_POST, 1);
  curl_setopt($curl, CURLOPT_POSTFIELDS, $fields);

  $curl_exec_result = curl_exec($curl);
  curl_close($curl);

  return $curl_exec_result;
}

function chess_twitchtv_stream_api_json_decode_result_check(
  $json_decode_result,
  $url,
  $client_id,
  $client_secret
) {
  if (json_last_error() === JSON_ERROR_NONE) {
    if (!empty($json_decode_result['error']) && $json_decode_result['error'] === 'Unauthorized') {
      $token = chess_twitchtv_stream_api_get_token($client_id, $client_secret);
      if ($token === NULL) {
        return FALSE;
      }
      else {
        $curl_exec_result = chess_twitchtv_stream_api_curl_exec_get(
          $url,
          $client_id,
          $token
        );
        $json_decode_result = json_decode($curl_exec_result, TRUE);
        if (json_last_error() !== JSON_ERROR_NONE) {
          return NULL;
        }
      }
    }

    return $json_decode_result;
  }
  else {
    return NULL;
  }
}
