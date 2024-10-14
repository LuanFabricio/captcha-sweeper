import Platform from './platform.js';

function make_env(env) {
	return new Proxy(env, {
		get(_, prop, __) {
			if (env[prop] !== undefined) {
				return env[prop].bind(env);
			}
			return (...args) => { console.error(`NOT IMPLEMENTED: ${prop} ${args}`)};
		}
	});
}

class Wasm {
	constructor() {
		this.ctx = this._fetch_canvas_ctx();
		this.platform = new Platform(this.ctx);
	}

	/**
	* @returns {CanvasRenderingContext2D}
	* */
	_fetch_canvas_ctx() {
		/** @type {HTMLCanvasElement} */
		const $canvas = document.querySelector("canvas#captcha");
		return $canvas.getContext("2d");
	}

	async load_wasm() {
		const wasm = await WebAssembly.instantiateStreaming(
			fetch("./main.wasm"), {	env: make_env(this.platform) }
		);

		this.wasm = wasm;
	}

	start() {
		if (this.wasm == undefined) {
			throw new Error("Wasm undefined.");
		}

		// this.wasm.instance.exports.main();
		this.platform.ctx.canvas.width = this.width;
		this.platform.ctx.canvas.height = this.height;

		const next = () => {
			// this.platform.loop_fn();
			window.requestAnimationFrame(next);
		};

		window.requestAnimationFrame(_ => {
			window.requestAnimationFrame(next);
		});
	}
}

const wasm = new Wasm();
wasm.load_wasm().then(_ => {
	wasm.platform.buffer = wasm.wasm.instance.exports.memory.buffer;
	wasm.platform.func_table = wasm.wasm.instance.exports.__indirect_function_table
	wasm.start();
});
