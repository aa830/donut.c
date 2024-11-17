class Donut < Formula
  desc "The original donut.c, but better!"
  homepage "https://github.com/aa830/donut.c"
  url "https://github.com/aa830/homebrew-donut.c/archive/refs/tags/v0.1.1.tar.gz"
  sha256 "a7fa16933e5a10bcb07c205af38f415db113619f3f95cd7f89accb646bea5848"
  version "v0.1.1"

  def install
    # Compile the project using `make`
    system "make"  # Ensure a `Makefile` exists in the project root directory

    # Install the compiled binary to Homebrew's bin directory
    bin.install "donut"

    # Install the man page
    man1.install "man/man1/donut.1"
  end

  test do
    # Simple test to verify the installation
    system "#{bin}/donut", "--help"
  end
end
