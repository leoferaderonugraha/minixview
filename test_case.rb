path = './test_cases/'

if (!Dir.exist?(path)) then
  Dir.mkdir(path)
end

(1..16).each do |i|
  File.open("./test_cases/test_case#{i}", 'w') do |fp|
    fp.write(((64+i).chr)*i)
  end
  print `./mxview -f ./test_cases/test_case#{i}`
end
