export default class Platform {

	/**
	* @param {CanvasRenderingContext2D} ctx 
	* @param {WebAssembly} ctx 
	* */
	constructor(ctx) {
		this.ctx = ctx;
		this.buffer = undefined;
		this.function_table = undefined;
		this.width = 0;
		this.height = 0;
	}

	/**
	* @param {number} str_ptr 
	* @param {number} w 
	* @param {number} h 
	* */
	platform_init_window(str_ptr, w, h) {
		this.width = w;
		this.height = h;

		this.ctx.canvas.width = this.width;
		this.ctx.canvas.height = this.height;
		document.title = cstr_by_ptr(this.buffer, str_ptr);
	}

	/** @param {number} color  */
	platform_clear_background(color) {
		this.ctx.fillStyle = color_to_hex(color);
		this.ctx.clearRect(0, 0, this.width, this.height);
		this.ctx.fillRect(0, 0, this.width, this.height);
	}

	/**
	* @returns {number}
	* */
	platform_rand() {
		return Math.ceil(Math.random() * 2147483647);
	}

	/** @param {Number} fn_ptr */
	platform_set_loop(fn_ptr) {
		this.loop_fn = this.func_table.get(fn_ptr);
	}

	/**
	* @param {number} x 
	* @param {number} y
	* @param {number} w 
	* @param {number} h
	* @param {number} color
	* */
	platform_draw_rectangle(x, y, w, h, color) {
		this.ctx.fillStyle = color_to_hex(color);
		this.ctx.fillRect(x, y, w, h);
	}

	/**
	* @param {number} x 
	* @param {number} y
	* @param {number} w 
	* @param {number} h
	* @param {number} color
	* */
	platform_draw_rectangle_lines(x, y, w, h, color) {
		this.ctx.strokeStyle = color_to_hex(color);
		this.ctx.strokeRect(x, y, w, h);
	}

	platform_rand_seed() {}
	platform_begin_drawing() {}
	platform_end_drawing() {}
	platform_close_drawing() {}
}

/**
* @param {number} color 
* */
function color_to_hex(color) {
	const r = ((color>>(0*8))&0xff).toString(16).padStart(2, "0");
	const g = ((color>>(1*8))&0xff).toString(16).padStart(2, "0");
	const b = ((color>>(2*8))&0xff).toString(16).padStart(2, "0");
	const a = ((color>>(3*8))&0xff).toString(16).padStart(2, "0");
	return '#'+r+g+b+a;
}

/**
* @param {ArrayBuffer} buffer 
* @param {number} str_ptr 
* @returns {string}
* */
function cstr_by_ptr(buffer, str_ptr) { 
	const mem = new Uint8Array(buffer);
	const len = cstr_len(mem, str_ptr);
	const str_bytes = new Uint8Array(buffer, str_ptr, len);

	return new TextDecoder().decode(str_bytes);
}

/**
* @param {Uint8Array} buffer 
* @param {number} str_ptr 
* @returns {number}
* */
function cstr_len(mem, str_ptr) {
	let len = 0;
	while (mem[str_ptr++] != 0) {
		len++;
	}
	return len;
}
