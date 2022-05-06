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

	void setQueen(int row, int column) {
		this.chessBoard[row][column] = 'Q';
	}

	void emptySquare(int row, int column) {
		if (this.chessBoard[row][column] == 'Q') {
		}
		this.chessBoard[row][column] = 'o';
	}

	void setDanger(int row, int column, char marker) {
		for (int i = 0; i < 8; i++) {
			if (i != row && this.chessBoard[i][column] == 'o') {
				this.chessBoard[i][column] = marker;
			}
		}
		for (int j = 0; j < 8; j++) {
			if (j != column && this.chessBoard[row][j] == 'o') {
				this.chessBoard[row][j] = marker;
			}
		}
		for (int i = row + 1, j = column + 1; i < 8 && j < 8; i++, j++) {
			if (this.chessBoard[i][j] == 'o') {
				this.chessBoard[i][j] = marker;
			}
		}
		for (int i = row - 1, j = column - 1; i >= 0 && j >= 0; i--, j--) {
			if (this.chessBoard[i][j] == 'o') {
				this.chessBoard[i][j] = marker;
			}
		}
		for (int i = row - 1, j = column + 1; i >= 0 && j < 8; i--, j++) {
			if (this.chessBoard[i][j] == 'o') {
				this.chessBoard[i][j] = marker;
			}
		}
		for (int i = row + 1, j = column - 1; i < 8 && j >= 0; i++, j--) {
			if (this.chessBoard[i][j] == 'o') {
				this.chessBoard[i][j] = marker;
			}
		}
	}

	void clearDanger(int row, int column, char marker) {
		for (int i = 0; i < 8; i++) {
			if (i != row && this.chessBoard[i][column] == marker) {
				this.chessBoard[i][column] = 'o';
			}
		}
		for (int j = 0; j < 8; j++) {
			if (j != column && this.chessBoard[row][j] == marker) {
				this.chessBoard[row][j] = 'o';
			}

		}
		for (int i = row + 1, j = column + 1; i < 8 && j < 8; i++, j++) {
			if (this.chessBoard[i][j] == marker) {
				this.chessBoard[i][j] = 'o';
			}
		}
		for (int i = row - 1, j = column - 1; i >= 0 && j >= 0; i--, j--) {
			if (this.chessBoard[i][j] == marker) {
				this.chessBoard[i][j] = 'o';
			}
		}
		for (int i = row - 1, j = column + 1; i >= 0 && j < 8; i--, j++) {
			if (this.chessBoard[i][j] == marker) {
				this.chessBoard[i][j] = 'o';
			}
		}
		for (int i = row + 1, j = column - 1; i < 8 && j >= 0; i++, j--) {
			if (this.chessBoard[i][j] == marker) {
				this.chessBoard[i][j] = 'o';
			}
		}
	}

	boolean setQueens(int queensRemaining) {

		if (queensRemaining < 0) {
			throw new IllegalArgumentException("value must be non-negative");
		}

		if (queensRemaining == 0) {
			for (int y = 0; y < 8; y++) {
				for (int x = 0; x < 8; x++) {
					if (chessBoard[x][y] != 'Q') {
						chessBoard[x][y] = 'o';
					}
				}
			}
			return true;
		}

		int queens = 0;
		for (int y = 0; y < 8; y++) {
			for (int x = 0; x < 8; x++) {
				if (chessBoard[x][y] == 'Q') {
					queens++;
					this.setDanger(x, y, (char) (46 - queens));
				}
			}
		}
		
		if (queens + queensRemaining > 8) {
			for (int j = 0; j < 8; j++) {
				for (int i = 0; i < 8; i++) {
					if (chessBoard[i][j] != 'Q') {
						chessBoard[i][j] = 'o';
					}
				}
			}
			return false;
		}
		
		queens++;

		for (int j = 0; j < 8; j++) {
			for (int i = 0; i < 8; i++) {

				if (chessBoard[i][j] == 'o') {
					this.setQueen(i, j);
					this.setDanger(i, j, (char) (46 - queens));

					if (this.setQueens(queensRemaining - 1)) {
						return true;
					} else {
						this.emptySquare(i, j);
						this.clearDanger(i, j, (char) (46 - queens));
					}
				}
			}
		}
		return false;
	}

	void showBoard() {
		for (int j = 0; j < 8; j++) {
			for (int i = 0; i < 8; i++) {
				System.out.print(chessBoard[i][j] + " ");
			}
			System.out.println();
		}
	}

	public static void main(String[] args) throws CloneNotSupportedException{
		EightQueens board = new EightQueens();
		System.out.println(board.setQueens(3));
		board.showBoard();
		System.out.println(board.setQueens(1));
		board.showBoard();
	
		EightQueens copy = (EightQueens)board.clone();
		copy.setQueen(1, 0);
		board.showBoard();
		copy.showBoard();
	}

}

