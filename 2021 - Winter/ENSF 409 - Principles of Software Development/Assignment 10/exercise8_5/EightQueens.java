public class EightQueens implements Cloneable {

	private char[][] chessBoard = new char[8][8];

	EightQueens() {
		for (int j = 0; j < 8; j++) {
			for (int i = 0; i < 8; i++) {
				chessBoard[i][j] = 'o';
			}
		}
	}

	public EightQueens(EightQueens origEightQueens) {
		for (int j = 0; j < 8; j++) {
			for (int i = 0; i < 8; i++) {
				this.chessBoard[i][j] = origEightQueens.chessBoard[i][j];
			}
		}
	}

	public Object clone() throws CloneNotSupportedException {
		EightQueens copy = (EightQueens) super.clone();
		copy.chessBoard = new char[8][8];
		for (int j = 0; j < 8; j++) {
			for (int i = 0; i < 8; i++) {
				copy.chessBoard[i][j] = this.chessBoard[i][j];
			}
		}
		return copy;
	}

	char[][] getBoard() {
		return this.chessBoard;
	}

	void emptySquare(int row, int column) {
		this.chessBoard[row][column] = 'o';
	}

	boolean setQueen(int row, int column) {
		if (this.chessBoard[row][column] != 'o') {
			return false;
		}
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				if (i == 0 && j == 0) {
					continue;
				}
				int n = 1;
				while (true) {
					int x = column + n * i;
					int y = row + n * j;
					if (x < 0 || x >= 8 || y < 0 || y >= 8) {
						break;
					}
					if (this.chessBoard[y][x] == 'Q') {
						return false;
					}
					n++;
				}
			}
		}
		this.chessBoard[row][column] = 'Q';
		return true;
	}

	void setDanger(int row, int column) {
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				if (i == 0 && j == 0) {
					continue;
				}
				int n = 1;
				while (true) {
					int x = column + n * i;
					int y = row + n * j;
					if (x < 0 || x >= 8 || y < 0 || y >= 8) {
						break;
					}
					this.chessBoard[y][x] = 'x';
					n++;
				}
			}
		}
	}

	void clearDanger() {
		for (int j = 0; j < 8; j++) {
			for (int i = 0; i < 8; i++) {
				if (chessBoard[i][j] == 'x') {
					chessBoard[i][j] = 'o';
				}
			}
		}
	}

	boolean setQueens(int queensRemaining) {
		if (queensRemaining > 8) {
			return false;
		}
		if (queensRemaining < 0) {
			throw new IllegalArgumentException("value must be non-negative");
		}
		if (queensRemaining == 0) {
			clearDanger();
			return true;
		}
		for (int j = 0; j < 8; j++) {
			for (int i = 0; i < 8; i++) {
				if (chessBoard[i][j] == 'Q') {
					this.setQueen(i, j);
					this.setDanger(i, j);
				}
			}
		}
		for (int j = 0; j < 8; j++) {
			for (int i = 0; i < 8; i++) {
				if (chessBoard[i][j] == 'o') {
					EightQueens next = new EightQueens(this);
					next.setQueen(i, j);
					next.setDanger(i, j);
					if (next.setQueens(queensRemaining - 1)) {
						this.chessBoard = next.chessBoard;
						return true;
					}
				}
			}
		}
		clearDanger();
		return false;
	}

	void showBoard() {
		for (int j = 0; j < 8; j++) {
			for (int i = 0; i < 8; i++) {
				System.out.print(chessBoard[i][j] + " ");
			}
			System.out.println();
		}
		System.out.println();
	}

	public static void main(String[] args) throws CloneNotSupportedException {
		EightQueens board = new EightQueens();
		System.out.println(board.setQueens(1));
		board.showBoard();
		System.out.println(board.setQueens(7));
		board.showBoard();

		EightQueens copy = (EightQueens) board.clone();
		System.out.println(copy.setQueen(0, 0));
		System.out.println(copy.setQueen(3, 3));
		board.showBoard();
		copy.showBoard();
	}

}
