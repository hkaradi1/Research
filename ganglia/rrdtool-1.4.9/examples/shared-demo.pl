#! /usr/bin/perl 


END {
  print "not ok 1\n" unless $loaded;
  unlink "demo.rrd";
}

sub ok
{
    my($what, $result) = @_ ;
    $ok_count++;
    print "not " unless $result;
    print "ok $ok_count $what\n";
}

#makes programm work AFTER install
use lib qw( /var/work/hkaradi1/gInstall/rrdtool/lib/perl );

use strict;
use vars qw(@ISA $loaded);

use RRDs;
$loaded = 1;
my $ok_count = 1;

ok("loading",1);

######################### End of black magic.

my $STEP  = 100;
my $RUNS  = 500;
my $GRUNS = 4;
my $RRD1  = "shared-demo.rrd";
my $RRD2  = "shared-demob.rrd";
my $PNG1  = "shared-demo1.png";
my $PNG2  = "shared-demo2.png";
my $time  = 30*int(time/30);
my $START = $time-$RUNS*$STEP;

my @options = ("-b", $START, "-s", $STEP,
 "DS:a:GAUGE:2000:U:U",
 "DS:b:GAUGE:200:U:U",
 "DS:c:GAUGE:200:U:U",
 "DS:d:GAUGE:200:U:U",
 "DS:e:DERIVE:200:U:U",
 "RRA:AVERAGE:0.5:1:5000",
 "RRA:AVERAGE:0.5:10:500");

print "* Creating RRD $RRD1 starting at $time.\n\n";
RRDs::create $RRD1, @options;

my $ERROR = RRDs::error;
ok("create A", !$ERROR);								#  2
if ($ERROR) {
  die "$0: unable to create `$RRD1': $ERROR\n";
}

print "* Creating RRD $RRD2 starting at $time.\n\n";
RRDs::create $RRD2, @options;

$ERROR= RRDs::error;
ok("create B",!$ERROR);								#  3
if ($ERROR) {
  die "$0: unable to create `$RRD2': $ERROR\n";
}

my $last = RRDs::last $RRD1;
if ($ERROR = RRDs::error) {
  die "$0: unable to get last `$RRD1': $ERROR\n";
}
ok("last A", $last == $START);							#  4

$last = RRDs::last $RRD2;
if ($ERROR = RRDs::error) {
  die "$0: unable to get last `$RRD2': $ERROR\n";
}
ok("last B", $last == $START);							#  5

print "* Filling $RRD1 and $RRD2 with $RUNS*5 values. One moment please ...\n";
print "* If you are running over NFS this will take *MUCH* longer\n\n";

srand(int($time / 100));

@options = ();

my $counter = 1e7;
for (my $t=$START+1;
     $t<$START+$STEP*$RUNS;
     $t+=$STEP+int((rand()-0.5)*7)){
  $counter += int(2500*sin($t/2000)*$STEP);
  my $data = (1000+500*sin($t/1000)).":".
      (1000+900*sin($t/2330)).":".
      (2000*cos($t/1550)).":".
      (3220*sin($t/3420)).":$counter";
  push(@options, "$t:$data");
  RRDs::update $RRD1, "$t:$data";
  if ($ERROR = RRDs::error) {
    die "$0: unable to update `$RRD1': $ERROR\n";
  }
}

RRDs::update $RRD2, @options;

if ($ERROR = RRDs::error) {
  die "$0: unable to update `$RRD2': $ERROR\n";
}

print "* Creating $GRUNS graphs: $PNG1 & $PNG2\n\n";
my $now = $time;
for (my $i=0;$i<$GRUNS;$i++) {
  my @rrd_pngs = ($RRD1, $PNG1, $RRD2, $PNG2);
  while (@rrd_pngs) {
    my $RRD = shift(@rrd_pngs);
    my $PNG = shift(@rrd_pngs);
    my ($graphret,$xs,$ys) = RRDs::graph $PNG, "--title", 'Test GRAPH', 
    '--base', '1024',
          "--vertical-label", 'Dummy Units', "--start", (-$RUNS*$STEP),
          "--end", $time,
	  "--interlace", "--imgformat","PNG",
          "DEF:alpha=$RRD:a:AVERAGE",
          "DEF:beta=$RRD:b:AVERAGE",
          "DEF:gamma=$RRD:c:AVERAGE",
          "DEF:delta=$RRD:d:AVERAGE",
          "DEF:epsilon=$RRD:e:AVERAGE",
          "CDEF:calc=alpha,beta,+,2,/,100,*,102,/",
          "AREA:alpha#0022e9:Short",
          "GPRINT:calc:MAX:Max calc %1.2lf",
          "STACK:beta#00b871:Demo Text",
          "GPRINT:calc:AVERAGE:Average calc %1.2lf",
          "STACK:beta#0ad871:Demo Text 2",
          "LINE1:gamma#ff0000:Line 1",
          "LINE2:delta#888800:Line 2",
          "LINE3:calc#00ff44:Line 3",
          "LINE3:epsilon#000000:Line 4",
          "HRULE:1500#ff8800:Horizontal Line at 1500",
          "PRINT:alpha:AVERAGE:Average Alpha %1.2lf",
          "PRINT:alpha:MIN:Min Alpha %1.2lf",
          "PRINT:alpha:MAX:Max Alpha %1.2lf",
          "GPRINT:calc:MIN:Min calc %1.2lf",
          "VRULE:".($now-3600)."#008877:60 Minutes ago",
          "VRULE:".($now-7200)."#008877:120 Minutes ago";

    if ($ERROR = RRDs::error) {
      die "ERROR: $ERROR\n";
    } else {
      print "Image Size: ${xs}x${ys}\n";
      print "Graph Return:\n",(join "\n", @$graphret),"\n\n";
    }
  }
}



my ($start,$step,$names,$array) = RRDs::fetch $RRD1, "AVERAGE";
$ERROR = RRDs::error;
die "ERROR: $ERROR\n" if $ERROR ;
print "start=$start, step=$step\n";
print "                    "; 
map {printf("%12s",$_)} @$names ;
print "\n";
foreach my $line (@$array){
  print "".localtime($start),"   ";
  $start += $step; 
  foreach my $val (@$line) {		
    printf "%12.1f", $val;
  }
  print "\n";
}



my ($start,$end,$step,$col_cnt,$legend,$data) = 
  RRDs::xport ("-m", 400,
	       "--start", "now-1day",
	       "--end", "now",
	       "DEF:alpha=$RRD1:a:AVERAGE",
	       "DEF:beta=$RRD1:d:AVERAGE",
	       "CDEF:calc=alpha,beta,+,2,/,100,*,102,/",
	       "XPORT:alpha:original ds",
	       "XPORT:calc:calculated values",
	       );

my $ERROR = RRDs::error;
die "$0: unable to xport: $ERROR\n" if $ERROR;

print "\nrrdxport test:\n\n";
print "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n\n";
print "<xport>\n";
print "  <meta>\n";
print "    <start>$start</start>\n";
print "    <step>$step</step>\n";
print "    <end>$end</end>\n";
print "    <rows>", $#$data + 1, "</rows>\n";
print "    <columns>$col_cnt</columns>\n";
print "    <legend>\n";
foreach my $entry (@$legend) {
    print "      <entry>$entry</entry>\n";
}
print "    </legend>\n";
print "  </meta>\n";
print "  <data>\n";
my $row_counter = 0;
foreach my $row (@$data) {
    $row_counter++;
    print "    <row id=\"$row_counter\"><t is=\"", scalar localtime($start), "\">$start</t>";
    $start += $step;
    foreach my $val (@$row) {
	printf ("<v>%1.10e</v>",$val) if $val ne '';
	print "<v>NaN</v>" if  $val eq '';
    }
    print "</row>\n";
}
print "  </data>\n";
print "</xport>\n";
