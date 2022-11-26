`define N 100

module top(input clk, stb, di, output do);
	localparam integer DIN_N = 4;
	localparam integer DOUT_N = `N;

	reg [DIN_N-1:0] din;
	wire [DOUT_N-1:0] dout;

	reg [DIN_N-1:0] din_shr;
	reg [DOUT_N-1:0] dout_shr;

	always @(posedge clk) begin
		din_shr <= {din_shr, di};
		dout_shr <= {dout_shr, din_shr[DIN_N-1]};
		if (stb) begin
			din <= din_shr;
			dout_shr <= dout;
		end
	end

	assign do = dout_shr[DOUT_N-1];

	roi roi (
		.clk(clk),
		.din(din),
		.dout(dout)
	);
endmodule

module roi(input clk, input [3:0] din, output [`N-1:0] dout);
	genvar i;
	generate
		for (i = 0; i < `N; i = i+1) begin:is
			LUT4 #(
				.INIT(16'h8001 + (i << 4))
			) lut (
				.I0(din[0]),
				.I1(din[1]),
				.I2(din[2]),
				.I3(din[3]),
				.O(dout[i])
			);
		end
	endgenerate
endmodule
