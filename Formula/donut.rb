class Donut < Formula
  desc "A spinning donut made with ASCII art"
  homepage "https://github.com/aa830/donut.c"
  url "file:///tmp/donut_build_with_binary.tar.gz"
  sha256 "d42075580760c0d2eb663d8f4cefaecd5438a046638e2d0c48037e1caeb7dfea"
  version "0.1a"

  def install
    bin.install "donut"  # This line must match the binary you want to install
  end

  test do
    system "#{bin}/donut", "--help"
  end
end
