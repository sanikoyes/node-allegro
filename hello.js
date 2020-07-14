const al = require('./build/Release/al');
const os = require('os')
const { Worker, isMainThread, parentPort } = require('worker_threads');


function main() {
	let filename = 'mysha.pcx'

	if (!al.init()) {
	//    abort_example("Could not init Allegro.\n");
	}

	// /* The second parameter to the process can optionally specify what
	//  * adapter to use.
	//  */
	// if (argc > 2) {
	//    al_set_new_display_adapter(atoi(argv[2]));
	// }

	// Allegro requires installing drivers for all input devices before they can be used.
	al.install_mouse()
	al.install_keyboard()

	// Initialize the image addon. Requires the allegro_image addon library.
	al.init_image_addon();

	// Create a new display that we can render the image to.
	let display = al.create_display(640, 480)
	if (display === undefined) {
	//    abort_example("Error creating display\n");
	}
	al.set_window_title(display, filename)

	// Load the image and time how long it took for the log.
	let t0 = al.get_time()
	let bitmap = al.load_bitmap(filename)
	let t1 = al.get_time()
	if (!bitmap) {
	   // abort_example("%s not found or failed to load\n", filename);
	}
	console.log(`Loading took ${t1 - t0} seconds`)

	// Create a timer that fires 30 times a second.
	let timer = al.create_timer(1.0 / 30);
	let queue = al.create_event_queue();
	console.log(`queue is ${queue}`)
	al.register_event_source(queue, al.get_keyboard_event_source())
	al.register_event_source(queue, al.get_display_event_source(display))
	al.register_event_source(queue, al.get_timer_event_source(timer))
	// Start the timer
	al.start_timer(timer)

	// Primary 'game' loop.
	let quiting = false
	let redraw = false;
	let zoom = 1;
	let event = al.ALLEGRO_EVENT()

	let tick

	let quit = () => {
		al.destroy_bitmap(bitmap)
		clearInterval(tick)
		al.uninstall_system()
		if (os.platform() !== 'win32') {
			parentPort.postMessage('quit')
		}
	}

	tick = setInterval(() => {
		// Wait for and get an event.
		al.wait_for_event(queue, event)
		// console.log(`${event.type()}`)

		switch (event.type()) {
			case 4: {
	//         int o = event.display.orientation;
	//         if (o == ALLEGRO_DISPLAY_ORIENTATION_0_DEGREES) {
	//             log_printf("0 degrees\n");
	//         }
	//         else if (o == ALLEGRO_DISPLAY_ORIENTATION_90_DEGREES) {
	//             log_printf("90 degrees\n");
	//         }
	//         else if (o == ALLEGRO_DISPLAY_ORIENTATION_180_DEGREES) {
	//             log_printf("180 degrees\n");
	//         }
	//         else if (o == ALLEGRO_DISPLAY_ORIENTATION_270_DEGREES) {
	//             log_printf("270 degrees\n");
	//         }
	//         else if (o == ALLEGRO_DISPLAY_ORIENTATION_FACE_UP) {
	//             log_printf("Face up\n");
	//         }
	//         else if (o == ALLEGRO_DISPLAY_ORIENTATION_FACE_DOWN) {
	//             log_printf("Face down\n");
	//         }
			} break;
			case 42: {
				quit()
			} break;
	//     /* Use keyboard to zoom image in and out.
	//      * 1: Reset zoom.
	//      * +: Zoom in 10%
	//      * -: Zoom out 10%
	//      * f: Zoom to width of window
	//      */
			case 11: {
				console.log('keycode:' + event.keyboard().keycode)
				console.log('unichar:' + event.keyboard().unichar)
				switch(event.keyboard().keycode) {
					// Break the loop and quite on escape key.
					case 59: {
						quit()
					} break
					case 47: {
						zoom = 1
					} break
					case 48: {
						zoom /= 1.1
					} break
					case 49: {
						zoom *= 1.1
					} break
					case 50: {
						zoom = al.get_display_width(display) / al.get_bitmap_width(bitmap)
					} break
				}
			} break;
	    	// Trigger a redraw on the timer event
			case 30: {
				redraw = true;
			} break;
		}
	    // Redraw, but only if the event queue is empty
	    if (redraw && al.is_event_queue_empty(queue)) {
	    	redraw = false;
	        // Clear so we don't get trippy artifacts left after zoom.
	        al.clear_to_color(al.map_rgb_f(0, 0, 0));
	        if (zoom === 1)
	        	al.draw_bitmap(bitmap, 0, 0, 0)
	        else
	        	al.draw_scaled_rotated_bitmap(bitmap, 0, 0, 0, 0, zoom, zoom, 0, 0)
	       	al.flip_display()
	    }

	}, 0)
}

if (os.platform() === 'win32') {
	main()
} else {
	if (isMainThread) {
		al.exec()

		let pump = setInterval(() => {
			al.pump()
		}, 1)

		let worker = new Worker(__filename);
		worker.on('message', msg => {
			if(msg === 'quit') {
				clearInterval(pump)
				// process.exit(1)
			}
		})

	} else {
		console.log('Run in worker', parentPort)
		main()
	}
}
