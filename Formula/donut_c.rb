class DonutC < Formula
  desc "A spinning donut in the terminal"
  homepage "https://github.com/aa830/donut.c"
  url "https://github.com/aa830/donut.c/archive/refs/tags/0.1a.tar.gz"
  sha256 "423d5b3822c0bd38ccd3c4ebd886855d14e6cfb7c38c7eef9019b2ded832ff69"
  license "MIT"

  def install
    system "make"
    bin.install "donut.c"
  end

  test do
    system "#{bin}/donut.c", "--help"
  end
end

