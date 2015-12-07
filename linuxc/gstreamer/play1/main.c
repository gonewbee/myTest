/**
 * @brief 使用gst_parse_launch解析playbin播放本地mp4
 *
 * 编译：gcc -Wall main.c $(pkg-config --cflags --libs gstreamer-1.0)
 */
#include <gst/gst.h>  
    
int main(int argc, char *argv[]) {  
  GstElement *pipeline;  
  GstBus *bus;  
  GstMessage *msg;  
    
  /* Initialize GStreamer */  
  gst_init (&argc, &argv);  
    
  /* Build the pipeline */  
  pipeline = gst_parse_launch ("playbin uri=file:///home/zsy/Downloads/test.mp4", NULL);  
    
  /* Start playing */  
  gst_element_set_state (pipeline, GST_STATE_PLAYING);  
    
  /* Wait until error or EOS */  
  bus = gst_element_get_bus (pipeline);  
  msg = gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_ERROR | GST_MESSAGE_EOS);  
    
  /* Free resources */  
  if (msg != NULL)  
    gst_message_unref (msg);  
  gst_object_unref (bus);  
  gst_element_set_state (pipeline, GST_STATE_NULL);  
  gst_object_unref (pipeline);  
  return 0;  
}  
