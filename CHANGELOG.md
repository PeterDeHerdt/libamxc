# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]


## Release 0.6.11 - 2020-09-03(05:13:31 +0000)

### Fixes

- Fixes g++ compilation warnings and errors

### Changes

- Add version prefix to support legacy build system

## Release 0.6.10 - 2020-09-01(19:34:50 +0000)

### Changes

- Add support for legacy tagging system with branch names in the tags
- Removes documentation generation jobs from the specific pipeline, they are now part of the default pipeline

## Release 0.6.9 - 2020-08-27(09:36:23 +0000)

### Changes

- Moves common re-usable macros to libamxc

## Release 0.6.8 - 2020-08-20(12:29:59 +0000)

### New

- Added amxc_string_is_numeric.

## Release 0.6.7 - 2020-08-14(21:31:27 +0000)

### Changes

- Improves hash table iterator `amxc_htable_for_each`, current iterator can be be (re)moved

## Release 0.6.6 - 2020-08-13(09:44:25 +0000)

### New 

-  variant list iterator macro
-  generic variant list join function

## Release 0.6.5 - 2020-08-04(04:55:03 +0000)

### Fixes

- Correct documentation tags

### Changes

- Update contributing guide

## Release 0.6.4 - 2020-07-24(10:58:59 +0000)

### Fixes

- Correctly add parts to linked list in string split functionality
- Make it possible to dump csv and ssv strings.
- Fixes taking amxc_string_t from variant

## Release 0.6.3 - 2020-07-22(18:05:43 +0000)

### Changes

- Adds RAW_VERSION to makefile.inc, VERSION must be X.Y.Z or X.Y.Z-HASH

## Release 0.6.2 - 2020-07-21(13:08:53 +0000)

### New

- String utility functions - resolve env variables and more

### Fixes

- Compilation issue with frotified musl

## Release 0.6.1 - 2020-07-15(13:45:56 +0000)

### New

- Adds amxc_string_copy function

## Release 0.6.0 - 2020-07-13(05:09:15 +0000)

### Changes

- String split and string join API's refactored and improved
- Update makefiles for SAH legacy build systems

## Release 0.5.5 - 2020-07-05(14:39:37 +0000)

### Fixes

- Memory leak in variant llist implementation and adds test

### Changes

- USes std=c11 instead of std=c18 for older compilers/toolchains

## Release 0.5.4 - 2020-07-01(20:04:43 +0000)

### Fixes

- `amxc_var_compare` results are wrong for some types

## Release 0.5.3 - 2020-06-30(07:45:20 +0000)

### Changes

- Scrubs Component.* files

## Release 0.5.2 - 2020-06-29(16:22:07 +0000)

### New 

- Support for legacy SAH build system

### Fixes

- Fixes compilation error with yocto & musl

## Release 0.5.1 - 2020-06-26(16:18:35 +0000)

### Fixes

- Fixes so name
- Fixes install target

## Release 0.5.0 - 2020-06-26(15:52:25 +0000)

### New 

- Variant type `csv_string` and `ssv_string`
- Copybara file

### Changes

- Builds targets into specific output directory

## Release 0.4.0 - 2020-06-19(10:53:02 +0000)

### New

- Timestamp data container
- Timestamp variant and conversions

### Changed

- Comma separate values string, splitting trims blanks from the individual parts

## Release 0.3.2 - 2020-06-15(11:02:26 +0000)

## New

- `amxc_variant_get_pathf` same as `amxc_var_get_path` but supports printf formatting for building path

## Release 0.3.1 - 2020-06-15(07:48:00 +0000)

### Fixes

- Issue #10 : comparing two empty string variants causes segmentation fault
- Adds tests to reproduce issue #10

### Changes

- update license to BSD+patent

## Release 0.3.0 - 2020-06-03(11:11:07 +0000)

### Changes

- Make it possible to delete current iterator from linked list while iterating over it using for_each macros (issue #9)
- Stops testing when test fails, preserve error code
- Collects unit test results

## Release 0.2.13 - 2020-05-28(06:36:19 +0000)

### Fixes

- Correct some typos in the documentation

## Release 0.2.12 - 2020-04-28(08:34:42 +0000)

### Fixes

- Returns empty string when string variant contains NULL string
- Crash when copying variant string containing NULL string
- Corrects documentation
- Fixes memory leak in test
- Fixes typo in documentation

## Release 0.2.11 - 2020-04-03(16:25:59 +0000)

### Fixes 

- Fixes amxc_string_prependf
- Fixes amxc_string_appendf - out of boundary bug

## Release 0.2.10 - 2020-04-01(17:51:34 +0000)

### Fixes

- Splitting string ending on separator string

## Release 0.2.9 - 2020-03-31(08:20:29 +0000)

### Changes

- API documentation - added documentation and fixes typos

## Release 0.2.8 - 2020-03-25(11:36:57 +0000)

### Changes

- Make var arg of function 'amxc_var_get_path' const

## Release 0.2.7 - 2020-03-11(06:26:56 +0000)

### Fixes

- Memory leak when adding duplicate keys to htable variant

## Release 0.2.6 - 2020-03-10(09:00:44 +0000)

### New

- variant types uint32_t and int32_t
- tests for new variant types
- Adds 'amxc_string_prependf' and 'amxc_string_vprependf' functions

### Changes

- Documentations generation configuration (doxygen)

## Release 0.2.5 - 2020-03-06(16:22:45 +0000)

### New

- Sortable linked list
- Linked list swap function
- amxc_string_vsetf, supports va_args

## Release 0.2.4 - 2020-03-01(15:14:23 +0000)

### New

- Feature - Sortable array
- API - new function `amxc_array_it_swap`
- API - new function `amxc_array_sort`

### Fixes

- DOC - doxygen documentation generation

### Changes

- CI/CD - Support ELK reporting
- CI/CD - Documentation generation
- CI/CD - Push HTML scan build and coverage reports to HTTP server

## Release 0.2.3 - 2020-02-22(21:40:08 +0000)

### Fixes

- Fixes issues found in analyses

### Changes

- Update .gitlab-ci.yml
- Corrects -Wl,-soname in linking, no version in soname

## Release 0.2.2 - 2020-02-19(12:59:11 +0000)

### Fixes

- Fix crash in amxc_var_take_amxc_string_t when variant contains NULL pointer
- Fix crash in amxc_string_delete

## Release 0.2.1 - 2020-02-18(10:30:37 +0000)

### New

- Linked lists and htable can be added to composite variant

## Changes

- Corrects documentation of function amxc_string_split_word_variant

## Release 0.2.0 - 2020-02-08(20:54:31 +0000)

### New

#### amxc_string_t

- adds 'amxc_string_setf' - resets amxc string and uses printf format to set the content
- adds 'amxc_string_appendf' - adds a string to an existing one using printf format
- adds 'amxc_string_join_variant_until' - joins parts into a string until a delimter is matched
- adds 'amxc_string_get_from_llist' - gets a string part (amxc_string_t) from a linked list
- adds 'amxc_string_get_text_from_llist' - gets a string part (const char *) from a linked list

#### amxc_var_t

- adds macro 'amxc_var_push' - resolves in amxc_var_push_<TYPE> functions
- adds 'amxc_var_take_amxc_string_t' - takes a amxc_string_t from variant (must be AMXC_VAR_ID_CSTRING)
- adds 'amxc_var_push_cstring_t' - pushes a 'char *' into a variant
- adds 'amxc_var_push_amxc_string_t' - pushes a 'amxc_string_t *' into a variant

### Changes

- renames function 'amxc_string_set_buffer' to 'amxc_string_push_buffer' (API consistency)
- do not strip binaries when installing (debugging feature)
- splitting a string in words keeps quotes in resulting list.

### Fixes

- amxc_string_new return value - int8_t -> int


## Release 0.2.0 - 2020-02-08(20:54:31 +0000)

### New

#### amxc_string_t

- adds 'amxc_string_setf' - resets amxc string and uses printf format to set the content
- adds 'amxc_string_appendf' - adds a string to an existing one using printf format
- adds 'amxc_string_join_variant_until' - joins parts into a string until a delimter is matched
- adds 'amxc_string_get_from_llist' - gets a string part (amxc_string_t) from a linked list
- adds 'amxc_string_get_text_from_llist' - gets a string part (const char *) from a linked list

#### amxc_var_t

- adds macro 'amxc_var_push' - resolves in amxc_var_push_<TYPE> functions
- adds 'amxc_var_take_amxc_string_t' - takes a amxc_string_t from variant (must be AMXC_VAR_ID_CSTRING)
- adds 'amxc_var_push_cstring_t' - pushes a 'char *' into a variant
- adds 'amxc_var_push_amxc_string_t' - pushes a 'amxc_string_t *' into a variant

### Changes

- renames function 'amxc_string_set_buffer' to 'amxc_string_push_buffer' (API consistency)
- do not strip binaries when installing (debugging feature)
- splitting a string in words keeps quotes in resulting list.

### Fixes

- amxc_string_new return value - int8_t -> int


## Release 0.1.4 - 2020-02-05(09:46:25 +0000)

## New

- Adds amxc_string_join_variant_until function

## Changes

- Gitlab CI/CD .gitlab-ci.yml change container image for building and testing

## Release 0.1.3 - 2020-02-03(06:50:04 +0000)

## New

- amxc_string_split functions
  - split string using separator into linked list of string or linked list of variants
  - split string into `words` into linked list of variants
- amxc_var_dump function
- wrapper functions to make it easier to add primitives into composite variant
- string take_buffer and set_buffer functions

## Fixes

- variant_htable and variant_list memory leaks, adds extra tests
- Adds set_buffer and take_buffer functions

## Release 0.1.2 - 2020-01-21(08:55:37 +0100)

### Fixes 

- variant type llist set index implementation
- gcc9.2 warning/error on strncpy 
- makefiles to accomodate archlinux pkgbuild
- improves & fixes set index implementation

### New

- variant type file descriptor

## Release 0.1.1 - 2020-01-15(13:30:02 +0000)

### New

- Adds trim functions and tests (issue 15)

## Release 0.1.0 - 2020-01-09(08:31:11 +0000)

### Fixes

- Added missing documentation (issue 17)
- Added missing tests (issue 18)
- Removed debug prints
- Fixed compilation errors (gcc 9.2.0)

### Documentation

- Removes common documents
- Updates links to common documents

### New

- Added fd variant implementation (issue 20)

## Release 0.0.9 - 2019-11-23(12:28:58 +0000)

### Fixes

- Code style
  - Fixes 13-assert-macros-assumes-exit-label-exists
  - Fixes all assert macros, label is passed as argument
  - Fixes typos in comment and change code layout

## Release 0.0.8 - 2019-11-16(15:24:15 +0000)

### Fixes

- issue 12 - Add AMXC_VAR_FLAG_UPDATE for amxc_var_set_index and amxc_var_set_key functions

## Release 0.0.7 - 2019-11-14(13:16:44 +0000)

### Added 

- `ANY` type for conversion functions
- `amxc_var_get_path` function - fetch from composed variant using dotted path

### Changed

- api changes - all getters (path,index,key) uses flag for copy or copyless

## Release 0.0.6 - 2019-11-10(19:57:26 +0000)

### Added

- `double` variant type
- Composed variants can be accessed by `key` or `index` using setters and getter functions
- Contribution guide - CONTRIBUTING.md
- Changelog - CHANGELOG.md

### Updated

- README.md
- Doxygen documentation
- Made code c18 compliant
