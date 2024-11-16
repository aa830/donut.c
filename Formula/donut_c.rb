class DonutC < Formula
  desc "A spinning donut in the terminal"
  homepage "https://github.com/aa830/donut.c"
  url "https://github.com/aa830/donut.c/archive/refs/tags/0.1a.tar.gz"
  sha256 "1541accb844e65eb011f26855cd30b637b377467be06d683d7fe5e5fd94df842"
  license "MIT"

  def install
    system "make"
    bin.install "donut.c"
  end

  test do
    system "#{bin}/donut.c", "--help"
  end
end

