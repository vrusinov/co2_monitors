# SPDX-FileCopyrightText: 2026 Vladimir Rusinov
# SPDX-License-Identifier: Apache-2.0
{
  description = "CO2 Monitors - ESPHome project with development environment";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-25.11";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      {
        devShells.default = pkgs.mkShell {
          buildInputs = with pkgs; [
            # ESPHome and dependencies
            esphome
            python312
            python312Packages.pip

            # Pre-commit hooks tools
            pre-commit
            clang-tools
            nodePackages.markdownlint-cli
            reuse
            typos
            cpplint
            proselint
            ruby
            bundler
            openssl
            zlib
            libffi
            readline
            libyaml
            gcc
            pkg-config
            curl

            # Utilities
            git
            gnumake
          ];

          shellHook = ''
            export LDFLAGS="-L${pkgs.openssl}/lib -L${pkgs.zlib}/lib -L${pkgs.libffi}/lib -L${pkgs.readline}/lib -L${pkgs.libyaml}/lib"
            export CPPFLAGS="-I${pkgs.openssl}/include -I${pkgs.zlib}/include -I${pkgs.libffi}/include -I${pkgs.readline}/include -I${pkgs.libyaml}/include"
            export LD_LIBRARY_PATH="${pkgs.lib.makeLibraryPath [pkgs.openssl pkgs.zlib pkgs.libffi pkgs.readline pkgs.libyaml]}:$LD_LIBRARY_PATH"

            # Ensure Ruby gems user bin is in PATH and install mdl if missing
            export GEM_HOME="$(ruby -e 'puts Gem.user_dir')"
            export PATH="$GEM_HOME/bin:$PATH"
            if ! command -v mdl >/dev/null 2>&1; then
              echo "Installing Ruby gem: mdl (MarkdownLint)"
              gem install --user-install mdl || {
                echo "Failed to install mdl gem" >&2
              }
            fi
            if ! ruby -e "require 'kramdown'" >/dev/null 2>&1; then
              echo "Installing Ruby gem: kramdown"
              gem install --user-install kramdown || {
                echo "Failed to install kramdown gem" >&2
              }
            fi
          '';
        };
      }
    );
}
