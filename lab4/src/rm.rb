#! /usr/bin/ruby
require 'find'

Find.find('./') do |path|
    if path.size > 6 and path[2].upcase == path[2]
        File::unlink(path)
    end
end