/*
 * xmas_snoopy.v
 *
 * vim: ts=4 sw=4
 *
 * Copyright (C) 2019-2020  Sylvain Munaut <tnt@246tNt.com>
 * SPDX-License-Identifier: CERN-OHL-P-2.0
 */

module xmas_snoopy (
	input  wire        pwr_usb_n,
	input  wire        pwr_chg_n,
	output wire        pwr_off,
	output wire [13:0] led_a,
	output wire [ 2:0] led_c,
);

	// Signals
	// -------

	// LED matrix
	reg  [15:0] led_cnt;

	wire  [3:0] led_a_sel;
	reg  [15:0] led_a_i;

	wire  [1:0] led_c_sel;
	reg   [2:0] led_c_i;

	// Auto-off
	reg  [17:0] ao_cnt;
	reg         ao_trig;

	// CRG
	wire        clk;
	reg   [3:0] rst_cnt;
	wire        rst;


	// LED Matrix
	// ----------

	// Cycle counter
	always @(posedge clk or posedge rst)
		if (rst)
			led_cnt <= 0;
		else
			led_cnt <= led_cnt + 1;

	assign led_ena   = (led_cnt[14:11] != 4'h0) && (led_cnt[14:11] != 4'h2);
	assign led_c_sel = led_cnt[5:4];
	assign led_a_sel = led_cnt[3:0];

	// Cathodes
	always @(posedge clk or posedge rst)
		if (rst)
			led_c_i <= 0;
		else begin
			led_c_i <= 0;
			led_c_i[led_c_sel] <= led_ena;
		end

	// Anodes
	always @(posedge clk or posedge rst)
		if (rst)
			led_a_i <= 0;
		else begin
			led_a_i <= 0;
			led_a_i[led_a_sel] <= 1'b1;
		end

	assign led_a = led_a_i[14:1];

	// Driver
	SB_RGBA_DRV #(
		.CURRENT_MODE("0b1"),
		.RGB0_CURRENT("0b000011"),  /* Green : 4 mA */
		.RGB1_CURRENT("0b000111"),  /* Pink  : 6 mA */
		.RGB2_CURRENT("0b001111")   /* Blue  : 8 mA */
	) led_cathode_drv_I (
		.RGBLEDEN (1'b1),
		.RGB0PWM  (led_c_i[0]),
		.RGB1PWM  (led_c_i[1]),
		.RGB2PWM  (led_c_i[2]),
		.CURREN   (1'b1),
		.RGB0     (led_c[0]),
		.RGB1     (led_c[1]),
		.RGB2     (led_c[2])
	);


	// Auto-OFF
	// --------

	// If VBUS not present for more than ~13 sec, power off
	always @(posedge clk or posedge rst)
		if (rst)
			ao_cnt <= 0;
		else
			ao_cnt <= (ao_cnt + 1) & {18{pwr_usb_n}};

	always @(posedge clk or posedge rst)
		if (rst)
			ao_trig <= 1'b0;
		else
			ao_trig <= ao_trig | ao_cnt[17];

	assign pwr_off = ao_trig;


	// Local CRG
	// ---------

	// Oscillator for button logic
	SB_LFOSC osc_I (
		.CLKLFPU (1'b1),
		.CLKLFEN (1'b1),
		.CLKLF   (clk)
	);

	// Reset
	always @(posedge clk)
		if (~rst_cnt[3])
			rst_cnt <= rst_cnt + 1;

	assign rst = ~rst_cnt[3];

endmodule // xmas_snoopy
