
cdata = configuration_data()
cdata.set_quoted('PACKAGE_VERSION', gst_version)
cdata.set_quoted('PACKAGE', 'gst-project-plugin')
cdata.set_quoted('GST_LICENSE', 'None')
cdata.set_quoted('GST_API_VERSION', api_version)
cdata.set_quoted('GST_PACKAGE_NAME', 'GStreamer project Plug-ins')
cdata.set_quoted('GST_PACKAGE_ORIGIN', 'https://gstreamer.freedesktop.org')
configure_file(output : 'config.h', configuration : cdata)

gstaudio_dep = dependency('gstreamer-audio-1.0',
    fallback: ['gst-plugins-base', 'audio_dep'])


adv_sources = [
  'gstadv.cpp',
  'gstadvaudiosrc.cpp'
  ]

gstaudiofilterexample = library('gstadv',
  adv_sources,
  cpp_args: project_args,
  include_directories : [configinc],
  dependencies : [gst_dep, gstbase_dep, gstaudio_dep],
  install : true,
  install_dir : plugins_install_dir,
)
