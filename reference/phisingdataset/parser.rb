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
  cleaned_line = line.gsub(/[()']/, '').chomp(',')
  # Parse the line as a CSV
  row = CSV.parse_line(cleaned_line)

  # Extract the second (URL) and fourth fields
  #puts "URL: #{row[1].strip}"


  begin
    url = URI.parse(row[1].strip).host
  rescue URI::InvalidURIError
    #puts "Invalid URI: #{row[1].strip}"
    next
  end

  int_field = row[3].to_i

  # Output or use these fields
  puts "#{url}, #{int_field}"
  cnt += 1
end

# Close the file
file.close
