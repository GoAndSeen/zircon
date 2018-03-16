// Copyright 2018 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <stddef.h>
#include <stdint.h>

#include <fbl/macros.h>
#include <fbl/string.h>
#include <fbl/unique_ptr.h>
#include <fbl/vector.h>
#include <zircon/types.h>

namespace blkctl {

class Command;

class BlkCtl final {
public:
    BlkCtl()
        : force_(false), argn_(0) {}
    ~BlkCtl() {}

    Command* cmd() { return cmd_ ? cmd_.get() : nullptr; }

    // Prints usage information based on the available |CommandSets|.
    void Usage() const;

    // Converts the command line arguments into a |Command| object and runs it..
    static zx_status_t Execute(int argc, char** argv);

    // Converts the command line arguments into a |Command| object and runs it..
    zx_status_t Parse(int argc, char** argv);

    // Convenience functions to get the successive arguments.  If the next argument is of the wrong
    // type, or is missing and the optional flag is not set, it will return |ZX_ERR_INVALID_ARGS|.
    // If the argument is missing, but |optional| is true, it will return |ZX_ERR_NOT_FOUND|.
    zx_status_t GetNumArg(const char* argname, uint64_t* out, bool optional = false);
    zx_status_t GetStrArg(const char* argname, const char** out, bool optional = false);

    // Rewinds the argument iterator by |n| arguments so they will be returned by |NextArg| again.
    zx_status_t UngetArgs(size_t n);

    // Checks that all arguments were consumed. Returns |ZX_ERR_INVALID_ARGS| if arguments remain.
    zx_status_t ArgsDone() const;

    // If "--force" has not been specified as an option, prompts the user to confirm the desired
    // action.  Returns ZX_ERR_CANCELED if the user does not confirm, ZX_OK otherwise.
    zx_status_t Confirm() const;

private:
    DISALLOW_COPY_AND_ASSIGN_ALLOW_MOVE(BlkCtl);

    // The parsed command
    fbl::unique_ptr<Command> cmd_;
    // The executable name
    fbl::String binname_;
    // Arbitrary additional arguments to a command.
    fbl::Vector<fbl::String> args_;
    // A flag to skip confirmation prompts
    bool force_;
    // Index to the next argument
    size_t argn_;
};

} // namespace blkctl