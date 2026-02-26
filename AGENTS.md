<!--
SPDX-FileCopyrightText: 2026 Vladimir Rusinov
SPDX-License-Identifier: Apache-2.0
-->

# Development Environment and Commits

## Always run git commits in the nix dev shell

**Always run git commits in the nix dev shell.** Run `nix flake enter` or use
`nix develop` to enter the development environment before making commits. This
ensures all pre-commit hooks and dependencies are available.

## Never Skip Pre-commit Hooks

**Do NOT use `git commit --no-verify` or skip pre-commit hooks in any way.**
All commits must pass the pre-commit checks. This ensures code quality and
consistency across the project.

**Do NOT run `pre-commit run --all-files`.**

**Do NOT fix the typos mutable rev warning.** The warning about typos
using `rev: v1` is expected and should be left as-is.
