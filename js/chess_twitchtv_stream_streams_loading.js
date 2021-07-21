jQuery(document).ready(function () {
  loadStream("begin");
});

function loadStream(streamId) {
  var isTimeout = false;
  var nextLoadName = "end";

  var getObject = jQuery.getJSON("/media/streams/stream/get/ajax/" + streamId, function (data) {
    nextLoadName = data.next;
    if (data.markup != "break") {
      var streamWrapper = jQuery("<div style='display: none;'>" + data.markup + "</div>");
      jQuery("#streams-content").prepend(streamWrapper);
      jQuery(streamWrapper).show(1000);

      isTimeout = true;
    }
  });


  getObject.always(function () {
    var randomVal = 0;
    if (isTimeout) {
      var minRandomVal = 1500;
      var maxRandomVar = 3000;
      randomVal = minRandomVal + Math.random() * (maxRandomVar + 1 - minRandomVal);
      randomVal = Math.floor(randomVal);
    }

    setTimeout(function () {
      if (nextLoadName != "end") {
        loadStream(nextLoadName);
      }
      else {
        jQuery("#magnify-wrapper").hide(500);

        if (jQuery(".stream-wrapper").length == 0) {
          jQuery("#empty-message-wrapper").show(500);
        }
      }
    }, randomVal);
  });
}
