require 'csv'
require 'uri'

cnt = 0

# Open the file
file = File.open('dataset.txt', 'r')

# Loop over each line in the file
file.each_line do |line|
  break if cnt == 100000
  next if line.include? "INSERT"

  # Remove brackets and trailing comma
  cleaned_line = (line.gsub(/^\(/, '').gsub(/\),$/, '')).strip

#  regex = /(\d+), '([^']*)', '([^']*)', (\d+), '([^']*)'/
  regex = /(\d+), '((?:[^'\\]|\\.)*)', '((?:[^'\\]|\\.)*)', (\d+), '((?:[^'\\]|\\.)*)'/
  matches = cleaned_line.match(regex)
  row = matches.captures
   
  # Extract the second (URL) and fourth fields
  begin
    url = URI.parse(row[1].strip).host
  rescue URI::InvalidURIError
    #puts "Invalid URI: #{row[1].strip}"
    next
  end

  int_field = row[3].to_i

  if int_field > 1
    puts "Invalid: #{url}, #{int_field}"
    p row
    exit(1)
  end

  # Output or use these fields
  puts "#{url}, #{int_field}"
  cnt += 1
end

# Close the file
file.close

