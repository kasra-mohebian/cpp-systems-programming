# A few tooling lessons

These are original notes from the introductory part of my C++ work, rather than a copy of the exercise questions or submitted answers.

- Git records changes; Make and CMake organize builds. A clean build directory helps distinguish source problems from stale build artifacts.
- `git merge --ff-only` accepts only a fast-forward. If histories diverge it refuses the merge; it does not enter a conflict-resolution merge.
- `git diff --stat` is a size summary. To inspect the actual changes, read the diff; to compare file contents, compare bytes or hashes.
- Public repositories deserve a fresh review of paths, contact information, generated files and licenses. A private submission history need not become a public history.
- Passing a test suite is evidence for the cases it exercises. Release builds, malformed inputs and sanitizers can reveal mistakes that a happy-path Debug run misses.
