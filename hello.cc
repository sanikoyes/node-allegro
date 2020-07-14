//
// Copyright (c) 2013-2016 Pavel Medvedev. All rights reserved.
//
// This file is part of v8pp (https://github.com/pmed/v8pp) project.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <node.h>
#include <nan.h>

#include <v8pp/class.hpp>

using namespace v8;

extern void register_allegro(Local<Object> exports);

void init(Local<Object> exports) {

	register_allegro(exports);

	Isolate* isolate = exports->GetIsolate();

	node::Environment* env = node::GetCurrentEnvironment(isolate->GetCurrentContext());

	node::AtExit(env, [](void* param)
	{
	  v8pp::cleanup(static_cast<Isolate*>(param));
	}, isolate);
}

#if NODE_MAJOR_VERSION >= 10
NAN_MODULE_WORKER_ENABLED(addon, init)
#else
NODE_MODULE(addon, init)
#endif
