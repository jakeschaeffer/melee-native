#!/usr/bin/env ruby
# Offline provenance check: pinned upstream commits must be actual ancestors,
# not merely copies of source files or a rewritten/squashed upstream history.
require 'json'
root = File.expand_path('../..', __dir__)
Dir.chdir(root)
lock = JSON.parse(File.read('native/upstream-lock.json'))
lock.each do |name, source|
  commit = source.fetch('commit')
  abort "Invalid commit for #{name}" unless /\A[0-9a-f]{40}\z/.match?(commit)
  unless system('git', 'merge-base', '--is-ancestor', commit, 'HEAD')
    abort "Missing #{name} ancestor #{commit}; fetch full history and complete the integration merge"
  end
  puts "PASS: #{name} #{commit} is included in HEAD"
end
puts 'This checks the pinned snapshots, not whether new upstream commits exist.'
