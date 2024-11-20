class Donut < Formula
  desc "The original donut.c, but better!"
  homepage "https://github.com/aa830/donut.c"
  url "https://github.com/aa830/homebrew-donut.c/archive/refs/tags/0.1.7.tar.gz"
  sha256 "867dacb595ebaf9c8e91464d8385685a6a9862e001c7cb317828b7bdedc0cd7f"
  version "0.1.7"

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
