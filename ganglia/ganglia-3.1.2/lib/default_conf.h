#ifndef DEFAULT_CONF_H
#define DEFAULT_CONF_H 1

char *default_gmond_configuration = NULL;

#define BASE_GMOND_CONFIGURATION "\
/* This configuration is as close to 2.5.x default behavior as possible\n\
   The values closely match ./gmond/metric.h definitions in 2.5.x */\n\
globals {\n\
  daemonize = yes\n\
  setuid = yes\n\
  user = nobody\n\
  debug_level = 0\n\
  max_udp_msg_len = 1472\n\
  mute = no\n\
  deaf = no\n\
  allow_extra_data = yes\n\
  host_dmax = 0 /*secs */\n\
  cleanup_threshold = 300 /*secs */\n\
  gexec = no\n\
  send_metadata_interval = 0\n\
}\n\
\n\
/*\n\
 * The cluster attributes specified will be used as part of the <CLUSTER>\n\
 * tag that will wrap all hosts collected by this instance.\n\
 */\n\
cluster {\n\
  name = \"unspecified\"\n\
  owner = \"unspecified\"\n\
  latlong = \"unspecified\"\n\
  url = \"unspecified\"\n\
}\n\
\n\
/* The host section describes attributes of the host, like the location */\n\
host {\n\
  location = \"unspecified\"\n\
}\n\
\n\
/* Feel free to specify as many udp_send_channels as you like.  Gmond\n\
   used to only support having a single channel */\n\
udp_send_channel {\n\
  mcast_join = 239.2.11.71\n\
  port = 8649\n\
  ttl = 1\n\
}\n\
\n\
/* You can specify as many udp_recv_channels as you like as well. */\n\
udp_recv_channel {\n\
  mcast_join = 239.2.11.71\n\
  port = 8649\n\
  bind = 239.2.11.71\n\
}\n\
\n\
/* You can specify as many tcp_accept_channels as you like to share\n\
   an xml description of the state of the cluster */\n\
tcp_accept_channel {\n\
  port = 8649\n\
}\n\
\n\
"

#define COLLECTION_GROUP_LIST "\
/* Each metrics module that is referenced by gmond must be specified and\n\
   loaded. If the module has been statically linked with gmond, it does\n\
   not require a load path. However all dynamically loadable modules must\n\
   include a load path. */\n\
modules {\n\
  module {\n\
    name = \"core_metrics\"\n\
  }\n\
  module {\n\
    name = \"cpu_module\"\n\
    path = \"modcpu.so\"\n\
  }\n\
  module {\n\
    name = \"disk_module\"\n\
    path = \"moddisk.so\"\n\
  }\n\
  module {\n\
    name = \"load_module\"\n\
    path = \"modload.so\"\n\
  }\n\
  module {\n\
    name = \"mem_module\"\n\
    path = \"modmem.so\"\n\
  }\n\
  module {\n\
    name = \"net_module\"\n\
    path = \"modnet.so\"\n\
  }\n\
  module {\n\
    name = \"proc_module\"\n\
    path = \"modproc.so\"\n\
  }\n\
  module {\n\
    name = \"sys_module\"\n\
    path = \"modsys.so\"\n\
  }\n\
}\n\
\n\
include ('/etc/ganglia/conf.d/*.conf')\n\
\n\
/* The old internal 2.5.x metric array has been replaced by the following\n\
   collection_group directives.  What follows is the default behavior for\n\
   collecting and sending metrics that is as close to 2.5.x behavior as\n\
   possible. */\n\
\n\
/* This collection group will cause a heartbeat (or beacon) to be sent every\n\
   20 seconds.  In the heartbeat is the GMOND_STARTED data which expresses\n\
   the age of the running gmond. */\n\
collection_group {\n\
  collect_once = yes\n\
  time_threshold = 20\n\
  metric {\n\
    name = \"heartbeat\"\n\
  }\n\
}\n\
\n\
/* This collection group will send general info about this host every\n\
   1200 secs.\n\
   This information doesn't change between reboots and is only collected\n\
   once. */\n\
collection_group {\n\
  collect_once = yes\n\
  time_threshold = 1200\n\
  metric {\n\
    name = \"cpu_num\"\n\
    title = \"CPU Count\"\n\
  }\n\
  metric {\n\
    name = \"cpu_speed\"\n\
    title = \"CPU Speed\"\n\
  }\n\
  metric {\n\
    name = \"mem_total\"\n\
    title = \"Memory Total\"\n\
  }\n\
  /* Should this be here? Swap can be added/removed between reboots. */\n\
  metric {\n\
    name = \"swap_total\"\n\
    title = \"Swap Space Total\"\n\
  }\n\
  metric {\n\
    name = \"boottime\"\n\
    title = \"Last Boot Time\"\n\
  }\n\
  metric {\n\
    name = \"machine_type\"\n\
    title = \"Machine Type\"\n\
  }\n\
  metric {\n\
    name = \"os_name\"\n\
    title = \"Operating System\"\n\
  }\n\
  metric {\n\
    name = \"os_release\"\n\
    title = \"Operating System Release\"\n\
  }\n\
  metric {\n\
    name = \"location\"\n\
    title = \"Location\"\n\
  }\n\
}\n\
\n\
/* This collection group will send the status of gexecd for this host\n\
   every 300 secs.*/\n\
/* Unlike 2.5.x the default behavior is to report gexecd OFF. */\n\
collection_group {\n\
  collect_once = yes\n\
  time_threshold = 300\n\
  metric {\n\
    name = \"gexec\"\n\
    title = \"Gexec Status\"\n\
  }\n\
}\n\
\n\
/* This collection group will collect the CPU status info every 20 secs.\n\
   The time threshold is set to 90 seconds.  In honesty, this\n\
   time_threshold could be set significantly higher to reduce\n\
   unneccessary  network chatter. */\n\
collection_group {\n\
  collect_every = 20\n\
  time_threshold = 90\n\
  /* CPU status */\n\
  metric {\n\
    name = \"cpu_user\"\n\
    value_threshold = \"1.0\"\n\
    title = \"CPU User\"\n\
  }\n\
  metric {\n\
    name = \"cpu_system\"\n\
    value_threshold = \"1.0\"\n\
    title = \"CPU System\"\n\
  }\n\
  metric {\n\
    name = \"cpu_idle\"\n\
    value_threshold = \"5.0\"\n\
    title = \"CPU Idle\"\n\
  }\n\
  metric {\n\
    name = \"cpu_nice\"\n\
    value_threshold = \"1.0\"\n\
    title = \"CPU Nice\"\n\
  }\n\
  metric {\n\
    name = \"cpu_aidle\"\n\
    value_threshold = \"5.0\"\n\
    title = \"CPU aidle\"\n\
  }\n\
  metric {\n\
    name = \"cpu_wio\"\n\
    value_threshold = \"1.0\"\n\
    title = \"CPU wio\"\n\
  }\n\
  /* The next two metrics are optional if you want more detail...\n\
     ... since they are accounted for in cpu_system.\n\
  metric {\n\
    name = \"cpu_intr\"\n\
    value_threshold = \"1.0\"\n\
    title = \"CPU intr\"\n\
  }\n\
  metric {\n\
    name = \"cpu_sintr\"\n\
    value_threshold = \"1.0\"\n\
    title = \"CPU sintr\"\n\
  }\n\
  */\n\
}\n\
\n\
collection_group {\n\
  collect_every = 20\n\
  time_threshold = 90\n\
  /* Load Averages */\n\
  metric {\n\
    name = \"load_one\"\n\
    value_threshold = \"1.0\"\n\
    title = \"One Minute Load Average\"\n\
  }\n\
  metric {\n\
    name = \"load_five\"\n\
    value_threshold = \"1.0\"\n\
    title = \"Five Minute Load Average\"\n\
  }\n\
  metric {\n\
    name = \"load_fifteen\"\n\
    value_threshold = \"1.0\"\n\
    title = \"Fifteen Minute Load Average\"\n\
  }\n\
}\n\
\n\
/* This group collects the number of running and total processes */\n\
collection_group {\n\
  collect_every = 80\n\
  time_threshold = 950\n\
  metric {\n\
    name = \"proc_run\"\n\
    value_threshold = \"1.0\"\n\
    title = \"Total Running Processes\"\n\
  }\n\
  metric {\n\
    name = \"proc_total\"\n\
    value_threshold = \"1.0\"\n\
    title = \"Total Processes\"\n\
  }\n\
}\n\
\n\
/* This collection group grabs the volatile memory metrics every 40 secs and\n\
   sends them at least every 180 secs.  This time_threshold can be increased\n\
   significantly to reduce unneeded network traffic. */\n\
collection_group {\n\
  collect_every = 40\n\
  time_threshold = 180\n\
  metric {\n\
    name = \"mem_free\"\n\
    value_threshold = \"1024.0\"\n\
    title = \"Free Memory\"\n\
  }\n\
  metric {\n\
    name = \"mem_shared\"\n\
    value_threshold = \"1024.0\"\n\
    title = \"Shared Memory\"\n\
  }\n\
  metric {\n\
    name = \"mem_buffers\"\n\
    value_threshold = \"1024.0\"\n\
    title = \"Memory Buffers\"\n\
  }\n\
  metric {\n\
    name = \"mem_cached\"\n\
    value_threshold = \"1024.0\"\n\
    title = \"Cached Memory\"\n\
  }\n\
  metric {\n\
    name = \"swap_free\"\n\
    value_threshold = \"1024.0\"\n\
    title = \"Free Swap Space\"\n\
  }\n\
}\n\
\n\
collection_group {\n\
  collect_every = 40\n\
  time_threshold = 300\n\
  metric {\n\
    name = \"bytes_out\"\n\
    value_threshold = 4096\n\
    title = \"Bytes Sent\"\n\
  }\n\
  metric {\n\
    name = \"bytes_in\"\n\
    value_threshold = 4096\n\
    title = \"Bytes Received\"\n\
  }\n\
  metric {\n\
    name = \"pkts_in\"\n\
    value_threshold = 256\n\
    title = \"Packets Received\"\n\
  }\n\
  metric {\n\
    name = \"pkts_out\"\n\
    value_threshold = 256\n\
    title = \"Packets Sent\"\n\
  }\n\
}\n\
\n\
/* Different than 2.5.x default since the old config made no sense */\n\
collection_group {\n\
  collect_every = 1800\n\
  time_threshold = 3600\n\
  metric {\n\
    name = \"disk_total\"\n\
    value_threshold = 1.0\n\
    title = \"Total Disk Space\"\n\
  }\n\
}\n\
\n\
collection_group {\n\
  collect_every = 40\n\
  time_threshold = 180\n\
  metric {\n\
    name = \"disk_free\"\n\
    value_threshold = 1.0\n\
    title = \"Disk Space Available\"\n\
  }\n\
  metric {\n\
    name = \"part_max_used\"\n\
    value_threshold = 1.0\n\
    title = \"Maximum Disk Space Used\"\n\
  }\n\
}\n\
\n\
"

#define SOLARIS_SPECIFIC_CONFIGURATION "\
/* solaris specific metrics begin */\n\
collection_group {\n\
  collect_every = 20\n\
  time_threshold = 90\n\
  metric {\n\
   name = \"rcache\"\n\
   value_threshold = 1.0\n\
  }\n\
  metric {\n\
   name = \"wcache\"\n\
   value_threshold = 1.0\n\
  }\n\
  metric {\n\
    name = \"bread_sec\"\n\
    value_threshold = 1.0\n\
  }\n\
  metric {\n\
    name = \"bwrite_sec\"\n\
    value_threshold = 1.0\n\
  }\n\
  metric {\n\
    name = \"lread_sec\"\n\
    value_threshold = 1.0\n\
  }\n\
  metric {\n\
    name = \"lwrite_sec\"\n\
    value_threshold = 1.0\n\
  }\n\
  metric {\n\
    name = \"phread_sec\"\n\
    value_threshold = 1.0\n\
  }\n\
  metric {\n\
    name = \"phwrite_sec\"\n\
    value_threshold = 1.0\n\
  }\n\
}\n\
/* end solaris specific metrics */\n\
\n\
"

#define HPUX_SPECIFIC_CONFIGURATION "\n\
collection_group {\n\
  collect_every = 40\n\
  time_threshold = 90\n\
  metric {\n\
    name = \"mem_arm\"\n\
    value_threshold = 1024.0\n\
  }\n\
  metric {\n\
    name = \"mem_rm\"\n\
    value_threshold = 1024.0\n\
  }\n\
  metric {\n\
    name = \"mem_avm\"\n\
    value_threshold = 1024.0\n\
  }\n\
  metric {\n\
    name = \"mem_vm\"\n\
    value_threshold = 1024.0\n\
  }\n\
}\n\
\n\
"

#endif
