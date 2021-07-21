<?php

module_load_include(
  'inc',
  'chess_twitchtv_stream',
  'includes/chess_twitchtv_stream_sort'
);
module_load_include(
  'inc',
  'chess_twitchtv_stream',
  'includes/chess_twitchtv_stream_api'
);

function chess_twitchtv_stream_admin_page_contents() {
  drupal_add_css(
    drupal_get_path(
      'module',
      'chess_twitchtv_stream'
    ) . '/css/chess_twitchtv_stream_admin.css'
  );

  $chess_twitchtv_stream_admin_markup = '';
  $chess_twitchtv_stream_admin_markup .= '<p>';
  $chess_twitchtv_stream_admin_markup .= 'Прямые онлайн видеотрансляции на шахматные темы.';
  $chess_twitchtv_stream_admin_markup .= '</p>';

  $output['chess_twitchtv_stream_admin_markup'] = [
    '#type' => 'markup',
    '#markup' => $chess_twitchtv_stream_admin_markup,
  ];

  $chess_twitchtv_stream_admin_stream_streams_form_content = drupal_get_form(
    'chess_twitchtv_stream_admin_stream_streams_form'
  );
  $output['chess_twitchtv_stream_admin_stream_streams_form_markup'] = [
    '#type' => 'markup',
    '#markup' => drupal_render(
      $chess_twitchtv_stream_admin_stream_streams_form_content
    ),
  ];

  $chess_twitchtv_stream_admin_stream_form_content = drupal_get_form(
    'chess_twitchtv_stream_admin_stream_form'
  );
  $output['chess_twitchtv_stream_admin_stream_form_markup'] = [
    '#type' => 'markup',
    '#markup' => drupal_render(
      $chess_twitchtv_stream_admin_stream_form_content
    ),
  ];

  return $output;
}

function chess_twitchtv_stream_admin_stream_streams_form($form, &$form_state) {
  form_load_include(
    $form_state,
    'inc',
    'chess_twitchtv_stream',
    'includes/chess_twitchtv_stream_admin_page_contents'
  );
  $query = db_select('chess_twitchtv_stream_streams');
  $query->fields(
    'chess_twitchtv_stream_streams',
    [
      'id',
      'weight',
      'user_name',
      'stream_name',
      'addition_time',
      'last_availability_time',
      'status',
    ]
  );
  $chess_twitchtv_stream_streams_results = $query->execute()->fetchAll();

  $form = [
    '#method' => 'post',
    '#prefix' => '<div id="chess-twitchtv-stream-admin-stream-streams-form-wrapper">',
    '#suffix' => '</div>',
  ];

  $form['stream-streams-add-button'] = [
    '#type' => 'button',
    '#name' => 'stream-streams-add-button',
    '#value' => 'Добавить',
    '#ajax' => [
      'callback' => 'chess_twitchtv_stream_admin_stream_streams_form_ajax',
      // 'wrapper' => 'stream-streams-form',
      'method' => 'replace',
      'effect' => 'fade',
      // 'progress' => array('type' => 'bar', 'message' => 'В процессе')
      'progress' => ['type' => 'throbber'],
    ],
    '#id' => 'stream-streams-add-button',
    '#prefix' => '<div id="stream-streams-list-wrapper">'
      . '<table id="stream-streams-list" width="100%">'
      . '<caption>'
      . '<div>'
      . 'Список потоков'
      . '&nbsp;&nbsp;&nbsp;'
      . '</div>',
    '#suffix' => '</caption>'
      . '<tr>'
      . '<th>ИД</th>'
      . '<th>Вес</th>'
      . '<th>Пользователь</th>'
      . '<th>Поток</th>'
      . '<th>Добавлен</th>'
      . '<th class="sorted-column">Доступен</th>'
      . '<th>Состояние</th>'
      . '<th>Изменение</th>'
      . '<th>Удаление</th>'
      . '</tr>',
  ];

  // $chess_twitchtv_stream_streams_results = array_reverse($chess_twitchtv_stream_streams_results);
  usort(
    $chess_twitchtv_stream_streams_results,
    'chess_twitchtv_stream_sort_by_time_desc'
  );

  $chess_twitchtv_stream_streams_results_count = count(
    $chess_twitchtv_stream_streams_results
  );
  for ($i = 0; $i < $chess_twitchtv_stream_streams_results_count; $i++) {
    $id = trim($chess_twitchtv_stream_streams_results[$i]->id);
    $weight = trim($chess_twitchtv_stream_streams_results[$i]->weight);
    $user_name = trim($chess_twitchtv_stream_streams_results[$i]->user_name);
    $stream_name = trim(
      $chess_twitchtv_stream_streams_results[$i]->stream_name
    );
    $addition_time = trim(
      $chess_twitchtv_stream_streams_results[$i]->addition_time
    );
    $last_availability_time = trim(
      $chess_twitchtv_stream_streams_results[$i]->last_availability_time
    );
    $status = (trim(
      $chess_twitchtv_stream_streams_results[$i]->status
    ) == '1' ? 'Online' : 'Offline');

    $form['stream-streams-change-button-' . $id] = [
      '#type' => 'button',
      '#name' => 'stream-streams-table-change-button-' . $id,
      '#value' => 'Изменить',
      '#ajax' => [
        'callback' => 'chess_twitchtv_stream_admin_stream_streams_form_ajax',
        'wrapper' => 'stream-streams-table-change-button-' . $id,
        'method' => 'append',
        'effect' => 'fade',
        // 'progress' => array('type' => 'bar', 'message' => 'В процессе')
        'progress' => ['type' => 'throbber'],
      ],
      '#id' => 'stream-streams-table-change-button-' . $id,
      '#prefix' => '<tr style="background-color: ' . ($status == 'Online' ? '#008000' : '#800000') . ';">'
        . '<td>'
        . $id
        . '</td>'
        . '<td>'
        . $weight
        . '</td>'
        . '<td>'
        . $user_name
        . '</td>'
        . '<td>'
        . $stream_name
        . '</td>'
        . '<td>'
        . date("d.m.Y - H:i:s", $addition_time)
        . '</td>'
        . '<td>'
        . date("d.m.Y - H:i:s", $last_availability_time)
        . '</td>'
        . '<td>'
        . $status
        . '</td>'
        . '<td>',
      '#suffix' => '</td>',
    ];

    $form['stream-streams-remove-button-' . $id] = [
      '#type' => 'button',
      '#name' => 'stream-streams-table-remove-button-' . $id,
      '#value' => 'Удалить',
      '#ajax' => [
        'callback' => 'chess_twitchtv_stream_admin_stream_streams_form_ajax',
        'wrapper' => 'stream-streams-table-remove-button-' . $id,
        'method' => 'append',
        'effect' => 'fade',
        // 'progress' => array('type' => 'bar', 'message' => 'В процессе')
        'progress' => ['type' => 'throbber'],
      ],
      '#id' => 'stream-streams-table-rem-button-' . $id,
      '#prefix' => '<td>',
      '#suffix' => '</td>'
        . '</tr>',
    ];
  }

  $form['stream-streams-table-close'] = [
    '#type' => 'markup',
    '#markup' => '</table>'
      . '</div>',
  ];

  return $form;
}

function chess_twitchtv_stream_admin_stream_streams_form_ajax(
  $form,
  &$form_state
) {
  ctools_include('modal');
  ctools_include('ajax');
  ctools_modal_add_js();

  $clicked_button_name = '';
  if (isset($form_state['clicked_button']['#name'])) {
    $clicked_button_name = trim($form_state['clicked_button']['#name']);
  }

  $commands = [];

  $action_name = '';
  if (preg_match('/-change-/u', $clicked_button_name) === 1) {
    $action_name = 'Изменение';
  }
  elseif (preg_match('/-remove-/u', $clicked_button_name) === 1) {
    $action_name = 'Удаление';
  }
  elseif (preg_match('/-add-/u', $clicked_button_name) === 1) {
    $action_name = 'Добавление';
  }

  switch ($action_name) {
    case 'Удаление':
    {
      $stream_id = '0';
      preg_match('/(?<=-)[0-9]+$/u', $clicked_button_name, $matches);
      if (isset($matches[0]) && is_numeric($matches[0])) {
        $stream_id = trim($matches[0]);
      }

      $chess_twitchtv_stream_remove_stream_form = drupal_get_form(
        'chess_twitchtv_stream_remove_stream_form'
      );
      $chess_twitchtv_stream_remove_stream_form['remove_form_stream_id_field']['#value'] = $stream_id;
      $chess_twitchtv_stream_remove_stream_form['remove_form_stream_markup']['#markup'] = '<p>Подтвердите удаление потока вещания с идентификатором ' . $stream_id . '</p><br />';

      $commands[] = ctools_modal_command_display(
        $action_name . ' потока вещания',
        $chess_twitchtv_stream_remove_stream_form
      );

      break;
    }
    case 'Изменение':
    {
      $stream_id = '0';
      preg_match('/(?<=-)[0-9]+$/u', $clicked_button_name, $matches);
      if (isset($matches[0]) && is_numeric($matches[0])) {
        $stream_id = trim($matches[0]);
      }

      $chess_twitchtv_stream_change_stream_form = drupal_get_form(
        'chess_twitchtv_stream_add_stream_form'
      );
      $chess_twitchtv_stream_change_stream_form['change_form_stream_id_field']['#value'] = $stream_id;
      $chess_twitchtv_stream_change_stream_form['change_form_action_name_field']['#value'] = 'change';

      $query = db_select('chess_twitchtv_stream_streams');
      $query->fields(
        'chess_twitchtv_stream_streams',
        [
          'weight',
          'user_id',
          'user_name',
          'stream_name',
        ]
      );
      $query->condition('id', $stream_id);
      $chess_twitchtv_stream_streams_results = $query->execute()->fetchAll();

      $chess_twitchtv_stream_change_stream_form['add_form_user_id_field']['#value'] = $chess_twitchtv_stream_streams_results[0]->user_id;
      $chess_twitchtv_stream_change_stream_form['add_form_user_name_field']['#value'] = $chess_twitchtv_stream_streams_results[0]->user_name;
      $chess_twitchtv_stream_change_stream_form['add_form_stream_name_field']['#value'] = $chess_twitchtv_stream_streams_results[0]->stream_name;
      $chess_twitchtv_stream_change_stream_form['add_form_weight_field']['#value'] = $chess_twitchtv_stream_streams_results[0]->weight;

      $commands[] = ctools_modal_command_display(
        $action_name . ' потока вещания',
        $chess_twitchtv_stream_change_stream_form
      );

      break;
    }
    case 'Добавление':
    {
      $commands[] = ctools_modal_command_display(
        $action_name . ' потока вещания',
        drupal_get_form('chess_twitchtv_stream_add_stream_form')
      );

      break;
    }
    default:
    {
    }
  }

  return [
    '#type' => 'ajax',
    '#commands' => $commands,
  ];
}

function chess_twitchtv_stream_admin_stream_form($form, &$form_state) {
  $query = db_select('chess_twitchtv_stream');
  $query->fields(
    'chess_twitchtv_stream',
    [
      'client_id',
      'client_secret',
      'text',
      'ad_content_first',
      'ad_content_second',
      'ad_content_third',
      'ad_content_fourth',
    ]
  );
  $chess_twitchtv_stream_results = $query->execute()->fetchAll();

  $client_id = (isset($chess_twitchtv_stream_results[0]->client_id) ? trim(
    $chess_twitchtv_stream_results[0]->client_id
  ) : '');
  $client_secret = (isset($chess_twitchtv_stream_results[0]->client_secret) ? trim(
    $chess_twitchtv_stream_results[0]->client_secret
  ) : '');
  $text = (isset($chess_twitchtv_stream_results[0]->text) ? trim(
    $chess_twitchtv_stream_results[0]->text
  ) : '');
  $ad_content_first = (isset($chess_twitchtv_stream_results[0]->ad_content_first) ? trim(
    $chess_twitchtv_stream_results[0]->ad_content_first
  ) : '');
  $ad_content_second = (isset($chess_twitchtv_stream_results[0]->ad_content_second) ? trim(
    $chess_twitchtv_stream_results[0]->ad_content_second
  ) : '');
  $ad_content_third = (isset($chess_twitchtv_stream_results[0]->ad_content_third) ? trim(
    $chess_twitchtv_stream_results[0]->ad_content_third
  ) : '');
  $ad_content_fourth = (isset($chess_twitchtv_stream_results[0]->ad_content_fourth) ? trim(
    $chess_twitchtv_stream_results[0]->ad_content_fourth
  ) : '');

  $form = [
    '#method' => 'post',
  ];

  $form['stream-fields-parameters-wrapper'] = [
    '#title' => 'Параметры',
    '#type' => 'fieldset',
    '#attributes' => ['class' => ['stream-fields-parameters-wrapper']],
    '#prefix' => '<div id="stream-fields-parameters-ajax-wrapper">',
    '#suffix' => '</div>',
  ];

  $form['stream-fields-parameters-wrapper']['stream-field-client-id'] = [
    '#title' => 'Client Id',
    '#description' => 'Идентификатор клиента для API Twitch',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => $client_id,
    '#attributes' => ['min' => '1'],
    '#size' => 128,
    '#maxlength' => 128,
  ];

  $form['stream-fields-parameters-wrapper']['stream-field-client-secret'] = [
    '#title' => 'Client Secret',
    '#description' => 'Секретный ключ клиента для API Twitch',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => $client_secret,
    '#attributes' => ['min' => '1'],
    '#size' => 128,
    '#maxlength' => 128,
  ];

  $form['stream-fields-parameters-wrapper']['stream-field-text'] = [
    '#title' => 'Текст',
    '#description' => 'Описание странницы или SEO-текст',
    '#type' => 'textarea',
    '#default_value' => $text,
    '#attributes' => [],
    '#rows' => 16,
  ];

  $form['stream-fields-parameters-wrapper']['stream-field-ad-1'] = [
    '#title' => 'Первый рекламный блок',
    '#description' => 'Код первого рекламного блока',
    '#type' => 'textarea',
    '#default_value' => $ad_content_first,
    '#attributes' => [],
    '#rows' => 10,
  ];

  $form['stream-fields-parameters-wrapper']['stream-field-ad-2'] = [
    '#title' => 'Второй второго блок',
    '#description' => 'Код второго рекламного блока',
    '#type' => 'textarea',
    '#default_value' => $ad_content_second,
    '#attributes' => [],
    '#rows' => 10,
  ];

  $form['stream-fields-parameters-wrapper']['stream-field-ad-3'] = [
    '#title' => 'Третий рекламный блок',
    '#description' => 'Код третьего рекламного блока',
    '#type' => 'textarea',
    '#default_value' => $ad_content_third,
    '#attributes' => [],
    '#rows' => 10,
  ];

  $form['stream-fields-parameters-wrapper']['stream-field-ad-4'] = [
    '#title' => 'Четвёртый рекламный блок',
    '#description' => 'Код четвёртого рекламного блока',
    '#type' => 'textarea',
    '#default_value' => $ad_content_fourth,
    '#attributes' => [],
    '#rows' => 10,
  ];

  $form['stream-fields-parameters-wrapper']['button'] = [
    '#type' => 'button',
    '#name' => 'stream-parameters-button',
    '#value' => 'Сохранить',
    '#ajax' => [
      'callback' => 'chess_twitchtv_stream_admin_stream_form_ajax',
      'wrapper' => 'stream-fields-parameters-ajax-wrapper',
      'method' => 'replace',
      'effect' => 'fade',
      // 'progress' => array('type' => 'bar', 'message' => 'В процессе')
      'progress' => ['type' => 'throbber'],
    ],
    '#id' => 'stream-form-ajax-submit',
  ];

  return $form;
}

function chess_twitchtv_stream_admin_stream_form_ajax($form, &$form_state) {
  $stream_field_client_id_value = trim(
    $form_state['values']['stream-field-client-id']
  );
  $stream_field_client_secret_value = trim(
    $form_state['values']['stream-field-client-secret']
  );
  $stream_field_text_value = trim($form_state['values']['stream-field-text']);
  $stream_field_ad_1_value = trim($form_state['values']['stream-field-ad-1']);
  $stream_field_ad_2_value = trim($form_state['values']['stream-field-ad-2']);
  $stream_field_ad_3_value = trim($form_state['values']['stream-field-ad-3']);
  $stream_field_ad_4_value = trim($form_state['values']['stream-field-ad-4']);

  $exe_result = NULL;
  $query = db_truncate('chess_twitchtv_stream');
  $exe_result = $query->execute();

  $query = db_insert('chess_twitchtv_stream');
  $query->fields(
    [
      'client_id' => $stream_field_client_id_value,
      'client_secret' => $stream_field_client_secret_value,
      'text' => $stream_field_text_value,
      'ad_content_first' => $stream_field_ad_1_value,
      'ad_content_second' => $stream_field_ad_2_value,
      'ad_content_third' => $stream_field_ad_3_value,
      'ad_content_fourth' => $stream_field_ad_4_value,
    ]
  );
  $exe_result = $query->execute();

  if ($exe_result == NULL) {
    // drupal_set_message('Нет изменений или произошла ошибка', 'error', false);
    drupal_get_messages(); // Очистка всех Drupal-сообщений
    $commands = [];
    $commands[] = ajax_command_remove('#stream-form-ajax-submit-message');
    $commands[] = ajax_command_before(
      '#page',
      '<div>' . theme(
        'status_messages'
      ) . '</div>'
    );
    $commands[] = ajax_command_after(
      '#stream-form-ajax-submit',
      '<div id="stream-form-ajax-submit-message" style="display: inline-block; color: #C00; background-color: rgba(204, 0, 0, 0.1); border: 1px solid #C00; border-radius: 5px; padding: 3px 17px;">Не удалось изменить</div>'
    );

    return [
      '#type' => 'ajax',
      '#commands' => $commands,
    ];
  }
  else {
    // drupal_set_message('Успешно изменено', 'status', false);
    drupal_get_messages(); // Очистка всех Drupal-сообщений
    $commands = [];
    $commands[] = ajax_command_remove('#stream-form-ajax-submit-message');
    $commands[] = ajax_command_before(
      '#page',
      '<div>' . theme(
        'status_messages'
      ) . '</div>'
    );
    $commands[] = ajax_command_after(
      '#stream-form-ajax-submit',
      '<div id="stream-form-ajax-submit-message" style="display: inline-block; color: #080; background-color: rgba(0, 136, 0, 0.1); border: 1px solid #080; border-radius: 5px; padding: 3px 17px;">Успешно изменено</div>'
    );

    return [
      '#type' => 'ajax',
      '#commands' => $commands,
    ];
  }

  drupal_set_message('Все поля обязательны для заполнения', 'error', FALSE);
}

function chess_twitchtv_stream_add_stream_form($form, &$form_state) {
  $form = [
    '#method' => 'post',
  ];

  $form['change_form_stream_id_field'] = [
    '#type' => 'hidden',
    '#default_value' => '',
    '#attributes' => [],
  ];

  $form['change_form_action_name_field'] = [
    '#type' => 'hidden',
    '#default_value' => '0',
    '#attributes' => [],
  ];

  $form['add_form_user_id_field'] = [
    '#type' => 'textfield',
    '#title' => 'Идентификатор пользователя',
    '#description' => 'Идентификатор пользователя, создавшего вещание',
    '#required' => FALSE,
    '#default_value' => '',
    '#size' => 36,
    '#maxlength' => 36,
    '#attributes' => [],
  ];

  $form['add_form_user_name_field'] = [
    '#type' => 'textfield',
    '#title' => 'Имя пользователя',
    '#description' => 'Имя пользователя, создавшего вещание',
    '#required' => TRUE,
    '#default_value' => '',
    '#size' => 36,
    '#maxlength' => 36,
    '#attributes' => [],
  ];

  $form['add_form_stream_name_field'] = [
    '#type' => 'textfield',
    '#title' => 'Имя потока',
    '#description' => 'Имя потока вещания',
    '#required' => TRUE,
    '#default_value' => '',
    '#size' => 36,
    '#maxlength' => 36,
    '#attributes' => [],
  ];

  $form['add_form_weight_field'] = [
    '#type' => 'textfield',
    '#title' => 'Вес потока',
    '#description' => 'Вес потока вещания',
    '#required' => FALSE,
    '#default_value' => '1000',
    '#size' => 36,
    '#maxlength' => 36,
    '#attributes' => [],
    '#disabled' => TRUE,
  ];

  $form['add_form_submit_button'] = [
    '#type' => 'button',
    '#name' => 'add-form-submit-button',
    '#value' => 'Отправить',
    '#ajax' => [
      'callback' => 'chess_twitchtv_stream_add_stream_form_ajax',
      'wrapper' => 'chess-twitchtv-stream-admin-stream-streams-form',
      'method' => 'replace',
      'effect' => 'fade',
      // 'progress' => array('type' => 'bar', 'message' => 'В процессе')
      'progress' => ['type' => 'throbber'],
    ],
  ];

  // Так как форма открывается в модальном окне, указываем файл (текущий) с функцией возврата
  form_load_include(
    $form_state,
    'inc',
    'chess_twitchtv_stream',
    'includes/chess_twitchtv_stream_admin_page_contents'
  );

  return $form;
}

function chess_twitchtv_stream_add_stream_form_ajax($form, &$form_state) {
  ctools_include('modal');
  ctools_include('ajax');
  ctools_modal_add_js();

  // Так как форма открывается в модальном окне, указываем файл (текущий) с функцией возврата
  form_load_include(
    $form_state,
    'inc',
    'chess_twitchtv_stream',
    'includes/chess_twitchtv_stream_admin_page_contents'
  );

  $commands = [];
  $commands[] = ctools_modal_command_dismiss();
  $commands[] = ajax_command_remove('#stream-streams-add-message');
  $commands[] = ajax_command_remove('#stream-streams-remove-message');

  $add_form_user_id_field = trim(
    $form_state['values']['add_form_user_id_field']
  );
  $add_form_user_name_field = trim(
    $form_state['values']['add_form_user_name_field']
  );
  $add_form_stream_name_field = trim(
    $form_state['values']['add_form_stream_name_field']
  );
  $add_form_weight_field = trim($form_state['values']['add_form_weight_field']);

  $stream_id = '0';
  if (isset($form_state['values']['change_form_stream_id_field']) && is_numeric(
      $form_state['values']['change_form_stream_id_field']
    )) {
    $stream_id = trim($form_state['values']['change_form_stream_id_field']);
  }

  $query = db_select('chess_twitchtv_stream_streams');
  $query->fields('chess_twitchtv_stream_streams', ['stream_name']);
  $query->condition('stream_name', $add_form_stream_name_field);
  $stream_exists_name_result = $query->execute()->fetchAll();
  $stream_exists_name = (empty($stream_exists_name_result) ? '' : $add_form_stream_name_field);

  $action_name = '';
  $stream_id_string = '';
  $stream_exists_name_other_id = 0;
  if (isset($form_state['values']['change_form_action_name_field']) && $form_state['values']['change_form_action_name_field'] == 'change') {
    $action_name = 'изменен';
    $stream_id_string = ' с идентификатором ' . $stream_id;

    if (!empty($stream_exists_name)) {
      $query = db_select('chess_twitchtv_stream_streams');
      $query->fields('chess_twitchtv_stream_streams', ['id']);
      $query->condition('stream_name', $add_form_stream_name_field);
      $stream_exists_name_other_id_result = $query->execute()->fetchAll();

      $stream_exists_name_other_id = trim(
        $stream_exists_name_other_id_result[0]->id
      );

      if (!(count(
          $stream_exists_name_other_id_result
        ) > 1 || $stream_exists_name_other_id != $stream_id)) {
        $stream_exists_name_other_id = 0;
      }
    }
  }
  else {
    $action_name = 'добавлен';
  }

  if (!empty($add_form_user_name_field) && !empty($add_form_stream_name_field)) {
    if ($action_name == 'изменен' && empty($stream_id)) {
      $commands[] = ajax_command_after(
        '#stream-streams-add-button',
        '<div id="stream-streams-remove-message" style="display: inline-block; color: #C00; background-color: rgba(204, 0, 0, 0.1); border: 1px solid #C00; border-radius: 5px; padding: 3px 17px;">Не получен идентификатор</div>'
      );
    }
    elseif (($action_name == 'добавлен' && !empty($stream_exists_name)) || ($action_name == 'изменен' && !empty($stream_exists_name_other_id))) {
      $commands[] = ajax_command_after(
        '#stream-streams-add-button',
        '<div id="stream-streams-remove-message" style="display: inline-block; color: #C00; background-color: rgba(204, 0, 0, 0.1); border: 1px solid #C00; border-radius: 5px; padding: 3px 17px;">Поток с именем "' . $stream_exists_name . '" уже добавлен</div>'
      );
    }
    else {
      if ($add_form_weight_field == '' || is_numeric($add_form_weight_field)) {
        $query = '';
        if ($action_name == 'изменен') {
          $query = db_update('chess_twitchtv_stream_streams');
          $query->condition('id', $stream_id);
        }
        else {
          $query = db_insert('chess_twitchtv_stream_streams');
        }

        $add_form_user_id_field = trim($add_form_user_id_field);
        if (empty($add_form_user_id_field)) {
          $query_select_chess_twitchtv_stream = db_select(
            'chess_twitchtv_stream'
          );
          $query_select_chess_twitchtv_stream->fields(
            'chess_twitchtv_stream',
            [
              'client_id',
              'client_secret',
              'token',
            ]
          );
          $chess_twitchtv_stream_results = $query_select_chess_twitchtv_stream->execute(
          )->fetchAll();

          $user_id = chess_twitchtv_stream_api_get_user_id(
            $chess_twitchtv_stream_results[0]->client_id,
            $chess_twitchtv_stream_results[0]->client_secret,
            $chess_twitchtv_stream_results[0]->token,
            $add_form_stream_name_field
          );

          if ($user_id === FALSE) {
            $commands[] = ajax_command_after(
              '#stream-streams-add-button',
              '<div id="stream-streams-add-message" style="display: inline-block; color: #C00; background-color: rgba(204, 0, 0, 0.1); border: 1px solid #C00; border-radius: 5px; padding: 3px 17px;">Невозможно получить новый токен.</div>'
            );

            drupal_get_messages(); // Очистка всех Drupal-сообщений
            $commands[] = ajax_command_before(
              '#page',
              '<div>' . theme(
                'status_messages'
              ) . '</div>'
            );

            return [
              '#type' => 'ajax',
              '#commands' => $commands,
            ];
          }
          else {
            if (!empty($user_id)) {
              $add_form_user_id_field = $user_id;
            }
          }
        }

        $query_fields = [
          'user_id' => $add_form_user_id_field,
          'user_name' => $add_form_user_name_field,
          'stream_name' => $add_form_stream_name_field,
        ];

        if (is_numeric($add_form_weight_field)) {
          $query_fields['weight'] = $add_form_weight_field;
        }

        if ($action_name == 'добавлен') {
          $addition_time = time();
          $query_fields['addition_time'] = $addition_time;
          $query_fields['last_availability_time'] = $addition_time;
        }

        // $query = db_update('chess_twitchtv_stream_streams');
        $query->fields($query_fields);
        $exe_result = $query->execute();

        if ($exe_result == NULL) {
          if ($action_name == 'изменен') {
            // drupal_set_message('Поток' . $stream_id_string . ' не ' . $action_name . '. Неизвестная ошибка.', 'warning', false);
            $commands[] = ajax_command_after(
              '#stream-streams-add-button',
              '<div id="stream-streams-add-message" style="display: inline-block; color: #FA0; background-color: rgba(255, 170, 0, 0.1); border: 1px solid #FA0; border-radius: 5px; padding: 3px 17px;">Поток' . $stream_id_string . ' не ' . $action_name . '. Изменения отсутствуют.</div>'
            );
          }
          else {
            // drupal_set_message('Поток' . $stream_id_string . ' не ' . $action_name . '. Неизвестная ошибка.', 'error', false);
            $commands[] = ajax_command_after(
              '#stream-streams-add-button',
              '<div id="stream-streams-add-message" style="display: inline-block; color: #C00; background-color: rgba(204, 0, 0, 0.1); border: 1px solid #C00; border-radius: 5px; padding: 3px 17px;">Поток' . $stream_id_string . ' не ' . $action_name . '. Неизвестная ошибка.</div>'
            );
          }
        }
        else {
          // drupal_set_message('Поток' . $stream_id_string . ' успешно ' . $action_name, 'status', false);
          $commands[] = ajax_command_replace(
            '#chess-twitchtv-stream-admin-stream-streams-form-wrapper',
            drupal_render(
              drupal_get_form('chess_twitchtv_stream_admin_stream_streams_form')
            )
          );
          $commands[] = ajax_command_after(
            '#stream-streams-add-button',
            '<div id="stream-streams-add-message" style="display: inline-block; color: #080; background-color: rgba(0, 136, 0, 0.1); border: 1px solid #080; border-radius: 5px; padding: 3px 17px;">Поток' . $stream_id_string . ' успешно ' . $action_name . '</div>'
          );
        }
      }
      else {
        // drupal_set_message('Поток' . $stream_id_string . ' не ' . $action_name . '. Поле "Вес потока" должно содержать целочисленное значение.', 'error', false);
        $commands[] = ajax_command_after(
          '#stream-streams-add-button',
          '<div id="stream-streams-add-message" style="display: inline-block; color: #C00; background-color: rgba(204, 0, 0, 0.1); border: 1px solid #C00; border-radius: 5px; padding: 3px 17px;">Поток' . $stream_id_string . ' не ' . $action_name . '. Поле "Вес потока" должно содержать целочисленное значение.</div>'
        );
      }
    }
  }
  else {
    // drupal_set_message('Поток' . $stream_id_string . ' не ' . $action_name . '. Не заполнены обязательные поля.', 'error', false);
    $commands[] = ajax_command_after(
      '#stream-streams-add-button',
      '<div id="stream-streams-add-message" style="display: inline-block; color: #C00; background-color: rgba(204, 0, 0, 0.1); border: 1px solid #C00; border-radius: 5px; padding: 3px 17px;">Поток' . $stream_id_string . ' не ' . $action_name . '. Не заполнены обязательные поля.</div>'
    );
  }

  drupal_get_messages(); // Очистка всех Drupal-сообщений
  $commands[] = ajax_command_before(
    '#page',
    '<div>' . theme(
      'status_messages'
    ) . '</div>'
  );

  return [
    '#type' => 'ajax',
    '#commands' => $commands,
  ];
}

function chess_twitchtv_stream_remove_stream_form($form, &$form_state) {
  $form = [
    '#method' => 'post',
  ];

  $form['remove_form_stream_id_field'] = [
    '#type' => 'hidden',
    '#default_value' => '0',
    '#attributes' => [],
  ];

  $form['remove_form_stream_markup'] = [
    '#type' => 'markup',
    // '#markup' => '<p>Подтвердите удаление потока вещания с ИД ' . $form_state['values']['remove_form_stream_id_field'] . '</p><br />'
    '#markup' => '',
  ];

  $form['yes-remove_form_submit_button'] = [
    '#type' => 'button',
    '#name' => 'yes-remove-form-submit-button',
    '#value' => 'Подтвердить',
    '#ajax' => [
      'callback' => 'chess_twitchtv_stream_remove_stream_form_ajax',
      'wrapper' => 'chess-twitchtv-stream-admin-stream-streams-form',
      'method' => 'replace',
      'effect' => 'fade',
      // 'progress' => array('type' => 'bar', 'message' => 'В процессе')
      'progress' => ['type' => 'throbber'],
    ],
  ];

  $form['no-remove_form_submit_button'] = [
    '#type' => 'button',
    '#name' => 'no-remove-form-submit-button',
    '#value' => 'Отменить',
    '#ajax' => [
      'callback' => 'chess_twitchtv_stream_remove_stream_form_ajax',
      'method' => 'replace',
      'effect' => 'fade',
      // 'progress' => array('type' => 'bar', 'message' => 'В процессе')
      'progress' => ['type' => 'throbber'],
    ],
  ];

  // Так как форма открывается в модальном окне, указываем файл (текущий) с функцией возврата
  form_load_include(
    $form_state,
    'inc',
    'chess_twitchtv_stream',
    'includes/chess_twitchtv_stream_admin_page_contents'
  );

  return $form;
}

function chess_twitchtv_stream_remove_stream_form_ajax($form, &$form_state) {
  ctools_include('modal');
  ctools_include('ajax');
  ctools_modal_add_js();

  // Так как форма открывается в модальном окне, указываем файл (текущий) с функцией возврата
  form_load_include(
    $form_state,
    'inc',
    'chess_twitchtv_stream',
    'includes/chess_twitchtv_stream_admin_page_contents'
  );

  $stream_id = 0;
  if (isset($form_state['values']['remove_form_stream_id_field']) && is_numeric(
      $form_state['values']['remove_form_stream_id_field']
    ) && $form_state['values']['remove_form_stream_id_field'] > 0) {
    $stream_id = trim($form_state['values']['remove_form_stream_id_field']);
  }

  $commands = [];
  $commands[] = ctools_modal_command_dismiss();
  $commands[] = ajax_command_remove('#stream-streams-add-message');
  $commands[] = ajax_command_remove('#stream-streams-remove-message');

  if (empty($stream_id)) {
    $commands[] = ajax_command_after(
      '#stream-streams-add-button',
      '<div id="stream-streams-remove-message" style="display: inline-block; color: #FA0; background-color: rgba(255, 170, 0, 0.1); border: 1px solid #FA0; border-radius: 5px; padding: 3px 17px;">Не получен идентификатор</div>'
    );
  }
  else {
    $clicked_button_name = '';
    if (isset($form_state['clicked_button']['#name'])) {
      $clicked_button_name = trim($form_state['clicked_button']['#name']);
    }

    drupal_get_messages(); // Очистка всех Drupal-сообщений
    $commands[] = ajax_command_before(
      '#page',
      '<div>' . theme(
        'status_messages'
      ) . '</div>'
    );

    $action_name = '';
    if (preg_match('/no-remove-/u', $clicked_button_name) === 1) {
      $action_name = 'Отменено';
    }
    elseif (preg_match('/yes-remove-/u', $clicked_button_name) === 1) {
      $action_name = 'Подтверждено';
    }

    switch ($action_name) {
      case 'Отменено':
      {
        $commands[] = ajax_command_after(
          '#stream-streams-add-button',
          '<div id="stream-streams-remove-message" style="display: inline-block; color: #FA0; background-color: rgba(255, 170, 0, 0.1); border: 1px solid #FA0; border-radius: 5px; padding: 3px 17px;">Отменено удаление потока с идентификатором ' . $stream_id . '</div>'
        );

        break;
      }
      case 'Подтверждено':
      {
        $query = db_delete('chess_twitchtv_stream_streams');
        $query->condition('id', $stream_id);
        $exe_result = $query->execute();

        if ($exe_result == NULL) {
          // drupal_set_message('Не удалось удалить поток', 'error', false);
          $commands[] = ajax_command_after(
            '#stream-streams-add-button',
            '<div id="stream-streams-remove-message" style="display: inline-block; color: #C00; background-color: rgba(204, 0, 0, 0.1); border: 1px solid #C00; border-radius: 5px; padding: 3px 17px;">Не удалось удалить поток с идентификатором ' . $stream_id . '</div>'
          );
        }
        else {
          // drupal_set_message('Поток успешно удалён', 'status', false);
          $commands[] = ajax_command_replace(
            '#chess-twitchtv-stream-admin-stream-streams-form-wrapper',
            drupal_render(
              drupal_get_form('chess_twitchtv_stream_admin_stream_streams_form')
            )
          );
          $commands[] = ajax_command_after(
            '#stream-streams-add-button',
            '<div id="stream-streams-remove-message" style="display: inline-block; color: #080; background-color: rgba(0, 136, 0, 0.1); border: 1px solid #080; border-radius: 5px; padding: 3px 17px;">Поток с идентификатором ' . $stream_id . ' успешно удалён</div>'
          );
        }

        break;
      }
      default:
      {
        $commands[] = ajax_command_after(
          '#stream-streams-add-button',
          '<div id="stream-streams-remove-message" style="display: inline-block; color: #FA0; background-color: rgba(255, 170, 0, 0.1); border: 1px solid #FA0; border-radius: 5px; padding: 3px 17px;">Не определено действие</div>'
        );
      }
    }
  }

  return [
    '#type' => 'ajax',
    '#commands' => $commands,
  ];
}
