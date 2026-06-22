#include <gst/gst.h>
#include <iostream>

int main(int argc, char *argv[]) {
  gst_init(&argc, &argv);

  GstElement *pipeline =
      gst_parse_launch("videotestsrc ! autovideosink", nullptr);

  if (!pipeline) {
    std::cerr << "Failed to create pipeline\n";
    return 1;
  }

  gst_element_set_state(pipeline, GST_STATE_PLAYING);

  std::cout << "Playing for 5 seconds...\n";
  g_usleep(5 * G_USEC_PER_SEC);

  gst_element_set_state(pipeline, GST_STATE_NULL);
  gst_object_unref(pipeline);

  return 0;
}
