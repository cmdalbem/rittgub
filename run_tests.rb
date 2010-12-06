#!/usr/bin/ruby

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
randomic = [0,1,2,5,nil]

tests.each_line do |t|
  t = t.split(/[\s\t]+/)
  candidates.each do |c|
    randomic.each do |r|
      puts "./bl #{t[1]} #{c} #{r} < data/#{t[0].upcase + ".IN2"}"
      system "./bl #{t[1]} #{c} #{r} < data/#{t[0].upcase + ".IN2"}"
    end
  end
end

