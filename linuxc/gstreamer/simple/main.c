/**
 * @breif 获取gstreamer版本
 *
 * 编译：gcc -Wall main.c $(pkg-config --cflags --libs gstreamer-1.0)
 */
#include <stdio.h>
#include <gst/gst.h>

int main (int   argc, char *argv[]){
  const gchar *nano_str;
  guint major, minor, micro, nano;

  gst_init (&argc, &argv);

  gst_version (&major, &minor, &micro, &nano);

  printf("%d %d %d %d\n", major, minor, micro, nano);

  if (nano == 1)
    nano_str = "(CVS)";
  else if (nano == 2)
    nano_str = "(Prerelease)";
  else
    nano_str = "";

  printf ("This program is linked against GStreamer %d.%d.%d %s\n",
          major, minor, micro, nano_str);

  return 0;
}
