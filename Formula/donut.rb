class Donut < Formula
  desc "The original donut.c, but better!"
  homepage "https://github.com/aa830/donut.c"
  url "https://github.com/aa830/homebrew-donut.c/archive/refs/tags/0.1d.tar.gz"
  sha256 "b8ed3978d5bdaf839727e8d3c745786ac1afa023c05a50486438859646611fb8"
  version "0.1d"

  def install
    # Compile the project using `make`
    system "make"  # Ensure a `Makefile` exists in the project root directory

    # Install the compiled binary to Homebrew's bin directory
    bin.install "donut"

    # Install the man page
    man1.install "man/man1/donut.c.1"
  end

  test do
    # Simple test to verify the installation
    system "#{bin}/donut", "--help"
  end
end
