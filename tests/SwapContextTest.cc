/* Copyright (c) 2015-2016 Stanford University
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR(S) DISCLAIM ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL AUTHORS BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "gtest/gtest.h"
#include "Arachne.h"

static const size_t testStackSize = 256;
static char stack[testStackSize];
static void* stackPointer;
static void *oldStackPointer;

static bool swapContextSuccess;

void swapContextHelper() {
    swapContextSuccess = 1;
    Arachne::swapcontext(&oldStackPointer, &stackPointer);
}

TEST(SwapContextTest, SwapContext) {
    swapContextSuccess = 0;
    stackPointer = stack + testStackSize;
    *reinterpret_cast<void**>(stackPointer) =
        reinterpret_cast<void*>(swapContextHelper);
    EXPECT_EQ(256, reinterpret_cast<char*>(stackPointer) -
            reinterpret_cast<char*>(stack));
    stackPointer = reinterpret_cast<char*>(stackPointer) -
        Arachne::SpaceForSavedRegisters;
    EXPECT_EQ(208, reinterpret_cast<char*>(stackPointer) -
            reinterpret_cast<char*>(stack));
    Arachne::swapcontext(&stackPointer, &oldStackPointer);
    EXPECT_EQ(1, swapContextSuccess);
}
