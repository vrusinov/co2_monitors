<!--
SPDX-FileCopyrightText: 2026 Vladimir Rusinov
SPDX-License-Identifier: Apache-2.0
-->

# Important: Never Skip Pre-commit Hooks

**Do NOT use `git commit --no-verify` or skip pre-commit hooks in any way.**
All commits must pass the pre-commit checks. This ensures code quality and
consistency across the project.

**Do NOT run `pre-commit run --all-files`.**

**Do NOT fix the typos mutable rev warning.** The warning about typos
using `rev: v1` is expected and should be left as-is.
