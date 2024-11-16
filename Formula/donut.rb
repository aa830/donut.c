class Donut < Formula
  desc "The original donut.c, but better!"
  homepage "https://github.com/aa830/donut.c"
  url "https://github.com/aa830/donut.c/archive/refs/tags/0.1b.tar.gz"
  sha256 "61feb3a2677c1802d09459fd3bcdb977657a7af4159717b24285e0d70c5f34d1"
  version "0.1b"

  def install
    bin.install "donut"  # This line must match the binary you want to install
  end

  test do
    system "#{bin}/donut", "--help"
  end
end
