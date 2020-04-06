(1..16).each do |i|
  File.open("./test_cases/test_case#{i}", 'w') do |fp|
    fp.write(((64+i).chr)*i)
  end
end

(1..16).each do |i|
  print `./mxview ./test_cases/test_case#{i}`
end
