class Donut < Formula
  desc "The original donut.c, but better!"
  homepage "https://github.com/aa830/donut.c"
  url "https://github.com/aa830/homebrew-donut.c/archive/refs/tags/0.1c.tar.gz"
  sha256 "3ca18d436df36b12ce223310a29808f5b5a7ee839a22cb943801eddb46d8613f"
  version "0.1c"

  def install
    bin.install "donut"  # This line must match the binary you want to install
      # Other installation steps for donut.c
    man1.install "man/man1/donut.c.1"  
      # Install the man page

    
  end

  test do
    system "#{bin}/donut", "--help"
  end
end
