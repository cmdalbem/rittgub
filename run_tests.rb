#!/usr/bin/ruby

if ARGV.size < 1
  puts 'Give me the name of the suite to run "top" or "naive"'
  exit
end

soia = 1

if ARGV[0] == 'top'
  soia = 1
elsif ARGV[0] == 'naive'
  soia = 0
else
  exit
end

tests =
"Arc111	27	5689
Arc111	9	16839
Barthol2 	51 	84
Barthol2 	27 	157
Warnecke 	10 	155
Warnecke 	20 	79
Scholl 	50 	1394
Scholl 	38 	1834
Scholl 	25 	2787
Wee-Mag 	20 	77
Wee-Mag 	30 	56
"
candidates = [0,25,50,75,100]

tests.each_line do |t|
  t = t.split(/[\s\t]+/)
  candidates.each do |c|
      puts "./bl #{t[1]} #{c} #{soia} < data/#{t[0].upcase + ".IN2"}"
      system "echo `./bl #{t[1]} #{c} #{soia} < data/#{t[0].upcase + ".IN2"}` #{t[2]}"
      puts '---'
  end
end

